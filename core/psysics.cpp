#include "psysics.h"
#include <Box2D/Box2D.h>
#include <map>
#include "event_system.h"

namespace Escape {
    struct ContactListener : public b2ContactListener {
        World *world;
        EventSystem *eventSystem;

        ContactListener(World *world, EventSystem *event_system) : world(world), eventSystem(event_system) {
        }

        void process(entt::entity a, entt::entity b) {
            eventSystem->enqueue(Collision{a, b});
        }

        void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {
            void *A = contact->GetFixtureA()->GetBody()->GetUserData();
            void *B = contact->GetFixtureB()->GetBody()->GetUserData();
            entt::entity a = *(entt::entity *) &A;
            entt::entity b = *(entt::entity *) &B;
            process(a, b);
            process(b, a);
        }
    };

    PhysicsSystem::PhysicsSystem() {
    }

    void PhysicsSystem::initialize() {
        ECSSystem::initialize();
        findSystem<EventSystem>()->listen([this](Impulse imp) {
            if (getWorld()->valid(imp.actor)) {
                auto &vel = getWorld()->get<Velocity>(imp.actor);
                vel += imp;
            }
        });

    }

    void PhysicsSystem::update(float delta) {
        ContactListener listener(getWorld(), findSystem<EventSystem>());
        b2World b2d_world(b2Vec2(0, 0));

        std::map<entt::entity, b2Body *> mapping;

        // Put terrain blocks into box2d
        getWorld()->view<Position>().each([&](auto ent, const auto &pos) {
            b2BodyDef bodyDef;
            bodyDef.position.Set(pos.x, pos.y);
            auto *vel = getWorld()->try_get<Velocity>(ent);
            if (vel) {
                bodyDef.linearVelocity.x = vel->x;
                bodyDef.linearVelocity.y = vel->y;
            }

            auto *rot = getWorld()->try_get<Rotation>(ent);
            if (rot) bodyDef.angle = rot->radian;

            bodyDef.type = b2_dynamicBody;

            if (getWorld()->has<AgentData>(ent))
                bodyDef.linearDamping = 15;

            b2FixtureDef fixtureDef;

            b2PolygonShape box_shape;
            b2CircleShape circle_shape;
            auto *rect = getWorld()->try_get<RectangleShape>(ent);
            auto *cir = getWorld()->try_get<CircleShape>(ent);
            if (rect) {
                box_shape.SetAsBox(rect->width / 2, rect->height / 2);
                fixtureDef.shape = &box_shape;
            } else if (cir) {
                circle_shape.m_radius = cir->radius;
                fixtureDef.shape = &circle_shape;
            } else {
                throw std::runtime_error("Nowadays terrain blocks only support rectangles and circles");
            }

            fixtureDef.density = 1;

            b2Body *wall = b2d_world.CreateBody(&bodyDef);
            wall->CreateFixture(&fixtureDef);
            wall->SetUserData((void *) (ent));
            mapping[ent] = wall;

        });

        b2d_world.SetContactListener(&listener);

        // set very fast options
        int velocityIterations = 1;
        int positionIterations = 1;
        b2d_world.SetSubStepping(false);
        b2d_world.SetContinuousPhysics(false);

        b2d_world.Step(delta, velocityIterations, positionIterations);

        getWorld()->view<Position>().each([&](auto ent, auto &pos) {
            b2Body *body = mapping[ent];
            if (getWorld()->has<Collidable>(ent)) {
                b2Vec2 position = body->GetPosition();
                pos = as<Position>(position);
            } else if (getWorld()->has<Velocity>(ent)) {
                auto &vel = getWorld()->get<Velocity>(ent);
                pos += vel * delta;
            }

            if (getWorld()->has<Velocity>(ent) && getWorld()->has<Collidable>(ent)) {
                b2Vec2 velocity = body->GetLinearVelocity();
                getWorld()->assign_or_replace<Velocity>(ent, as<Velocity>(velocity));
            }
        });
    }

}
