#include "psysics.h"
#include <Box2D/Box2D.h>
#include "control.h"
#include <map>

namespace Escape {
    struct ContactListener : public b2ContactListener {
        World *world;

        ContactListener(World *world) : world(world) {
        }

        void process(entt::entity a, entt::entity b) {

            if (!world->has<CollisionResults>(a)) {
                world->assign<CollisionResults>(a);
            }
            CollisionResults &results = world->get<CollisionResults>(a);
            results.results.push_back(Collision{.hit_with = b});
        }

        void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {
            void *A = contact->GetFixtureA()->GetBody()->GetUserData();
            void *B = contact->GetFixtureB()->GetBody()->GetUserData();
            entt::entity a = *(entt::entity *) &A;
            entt::entity b = *(entt::entity *) &B;
            process(a, b);
            process(b, a);
        }

        void PostPostProcess() {

        }

    };

    PhysicsSystem::PhysicsSystem() {
    }

    void PhysicsSystem::update(float delta) {
        ContactListener listener(getWorld());
        b2World b2d_world(b2Vec2(0, 0));

        std::map<entt::entity, b2Body *> mapping;

        getWorld()->clear<CollisionResults>();
        // Put walls into box2d
        getWorld()->view<Position, Rotation, TerrainData>().each([&](auto ent, auto &pos, auto &rot, auto &ter) {
            if (ter.type == TerrainType::BOX) {
                b2BodyDef wallDef;
                wallDef.position.Set(pos.x, pos.y);
                wallDef.angle = rot.radian;
                b2PolygonShape wallBox;
                wallBox.SetAsBox(ter.argument_1 / 2, ter.argument_2 / 2);
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &wallBox;
                fixtureDef.density = 0;
                fixtureDef.friction = 1e6f;
                b2Body *wall = b2d_world.CreateBody(&wallDef);
                wall->CreateFixture(&fixtureDef);
                wall->SetUserData((void *) (ent));

                mapping[ent] = wall;

            }
        });

        // put agents and bullets in box2d
        getWorld()->view<Position, Velocity, Hitbox>().each([&](auto ent, auto &pos, auto &vel, auto &hit) {
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(pos.x, pos.y);
            bodyDef.linearVelocity.x = vel.x;
            bodyDef.linearVelocity.y = vel.y;

            b2CircleShape circle;
            circle.m_radius = hit.radius;

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &circle;

            if (getWorld()->has<BulletData>(ent)) {
                BulletData &data = getWorld()->get<BulletData>(ent);
                fixtureDef.density = data.density;
                fixtureDef.friction = 1e6f;
//                 this slows down fps and is unnecessary for relatively slow speed.
//                bodyDef.bullet = true;
            } else {
                fixtureDef.density = 1;
                fixtureDef.friction = 0.1;
                bodyDef.linearDamping = 15;
            }

            b2Body *body = b2d_world.CreateBody(&bodyDef);
            body->CreateFixture(&fixtureDef);

            body->SetUserData((void *) (ent));
            mapping[ent] = body;
        });

        // Impulsion control
        getWorld()->view<Position, Velocity, Message<Impulse>>().each(
                [&](auto ent, Position &pos, auto &vel, auto &impulse) {
                    if (!impulse.processed) {
                        mapping[ent]->ApplyLinearImpulse(as<b2Vec2>(impulse.data), as<b2Vec2>(pos), true);
                        impulse.processed = true;
                    }
                });


        b2d_world.SetContactListener(&listener);

        // set very fast options
        int velocityIterations = 1;
        int positionIterations = 1;
        b2d_world.SetSubStepping(false);
        b2d_world.SetContinuousPhysics(false);

        b2d_world.Step(delta, velocityIterations, positionIterations);
        listener.PostPostProcess();

        // fetch data for agents
        getWorld()->view<Position, Velocity, AgentData>().each([&](auto ent, auto &pos, auto &vel, auto &agt) {
            b2Body *body = mapping[ent];
            b2Vec2 position = body->GetPosition();
            b2Vec2 velocity = body->GetLinearVelocity();
            pos = as<Position>(position);
            vel = as<Velocity>(velocity);
        });

        // Only movement
         getWorld()->view<Position, Velocity, BulletData>().each([&](auto ent, auto &pos, auto &vel, auto &agt) {
             pos += vel * delta;
         });
    }

}
