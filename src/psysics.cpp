#include "psysics.h"
#include <glm/glm.hpp>
#include <Box2D/Box2D.h>
#include <map>

namespace Escape {
    struct StickyInfo {
        b2Body *arrowBody;
        b2Body *targetBody;
    };

    struct ContactListener : b2ContactListener {
        std::vector<StickyInfo> m_collisionsToMakeSticky;

        void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {
            b2Fixture *fixtureA = contact->GetFixtureA();
            b2Fixture *fixtureB = contact->GetFixtureB();

            StickyInfo si;
            si.targetBody = fixtureA->GetBody();
            si.arrowBody = fixtureB->GetBody();
            m_collisionsToMakeSticky.push_back(si);
        }

        void process(World *world) {

            //in Step function, immediately after calling world step
            for (int i = 0; i < m_collisionsToMakeSticky.size(); i++) {
                StickyInfo &si = m_collisionsToMakeSticky[i];
                si.arrowBody->SetLinearVelocity(b2Vec2(0, 0));
                si.targetBody->SetLinearVelocity(b2Vec2(0, 0));
            }
            m_collisionsToMakeSticky.clear();
        }

    };

    PhysicsSystem::PhysicsSystem() {
    }

    void PhysicsSystem::update(clock_type delta) {
        b2World b2d_world(b2Vec2(0, 0));
        std::map<entt::entity, b2Body *> mapping;
        // Put walls into box2d
        world->view<Position, Rotation, TerrainData>().each([&](auto ent, auto &pos, auto &rot, auto &ter) {
            if (ter.type == TerrainType::BOX) {
                b2BodyDef wallDef;
                wallDef.position.Set(pos.x, pos.y);
                wallDef.angle = rot.radian;
                wallDef.userData = (void *) ent;
                b2PolygonShape wallBox;
                wallBox.SetAsBox(ter.arguments[0] / 2, ter.arguments[1] / 2);
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &wallBox;
                fixtureDef.density = 0;
                fixtureDef.friction = 1e6f;
                b2Body *wall = b2d_world.CreateBody(&wallDef);
                wall->CreateFixture(&fixtureDef);

                mapping[ent] = wall;
            }
        });

        // put agents and bullets in box2d
        world->view<Position, Velocity, Hitbox>().each([&](auto ent, auto &pos, auto &vel, auto &hit) {
            b2BodyDef bodyDef;
            bodyDef.userData = (void *) ent;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(pos.x, pos.y);
            bodyDef.linearVelocity.x = vel.x;
            bodyDef.linearVelocity.y = vel.y;
            // FIXME bullets not hitting
            // TODO add some listener
            b2CircleShape circle;
            circle.m_radius = hit.radius;

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &circle;

            if (world->has<BulletData>(ent)) {
                BulletData &data = world->get<BulletData>(ent);
                fixtureDef.density = data.density;
                fixtureDef.friction = 1e6f;
//                 this slows down fps and is unnecessary for relatively slow speed.
//                bodyDef.bullet = true;
            } else {
                fixtureDef.density = 1;
                fixtureDef.friction = 0.1;
                bodyDef.linearDamping = 15;
            }
            // FIXME bullet will still slip
            b2Body *body = b2d_world.CreateBody(&bodyDef);
            body->CreateFixture(&fixtureDef);

            mapping[ent] = body;
        });

        // Impulsion control
        world->view<Position, Velocity, Control<Impulse>>().each(
                [&](auto ent, Position &pos, auto &vel, auto &impulse) {
                    if (!impulse.processed) {
                        mapping[ent]->ApplyLinearImpulse(as<b2Vec2>(impulse.data), as<b2Vec2>(pos), true);
                        impulse.processed = true;
                    }
                });

        ContactListener listener;
        int velocityIterations = 1;
        int positionIterations = 1;
        b2d_world.SetSubStepping(false);
        b2d_world.SetContactListener(&listener);
        b2d_world.Step(delta, velocityIterations, positionIterations);
        listener.process(world);

        // fetch data
        world->view<Position, Velocity>().each([&](auto ent, auto &pos, auto &vel) {
            b2Body *body = mapping[ent];
            b2Vec2 position = body->GetPosition();
            b2Vec2 velocity = body->GetLinearVelocity();
            pos = as<Position>(position);
            vel = as<Velocity>(velocity);
        });

        // Only movement
        // world->view<Position, Velocity>().each([&](auto ent, auto &pos, auto &vel) {
        //     pos += vel * delta;
        // });
    }

}
