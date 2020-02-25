#include "movement.h"
#include <glm/glm.hpp>
#include <Box2D/Box2D.h>
#include <map>
Escape::MovementSystem::MovementSystem()
{
}

void Escape::MovementSystem::move(Entity ent, const Velocity &vel)
{
    if (!world->has<Velocity>(ent))
    {
        world->assign<Velocity>(ent, 0, 0);
    }
    world->replace<Velocity>(ent, [&](auto &v) {
        v *= 0.93f;
        v += vel * 0.7f;
    });
}

void Escape::MovementSystem::update(clock_type delta)
{
    b2World b2d_world(b2Vec2(0, 0));
    std::map<entt::entity, b2Body *> mapping;
    // Put walls into box2d
    world->view<Position, Rotation, TerrainData>().each([&](auto ent, auto &pos, auto &rot, auto &ter) {
        if (ter.type == TerrainType::BOX)
        {
            b2BodyDef wallDef;
            wallDef.position.Set(pos.x, pos.y);
            wallDef.angle = rot.radian;

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
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(pos.x, pos.y);
        bodyDef.linearVelocity.x = vel.x;
        bodyDef.linearVelocity.y = vel.y;
        b2Body *body = b2d_world.CreateBody(&bodyDef);
        b2FixtureDef fixtureDef;
        b2CircleShape circle;
        // FIXME bullets not hitting
        // TODO add some listener
        circle.m_radius = hit.radius * 0.95;
        fixtureDef.shape = &circle;
        if (world->has<BulletData>(ent))
        {
            fixtureDef.density = 7e3f;
            fixtureDef.friction = 1e6f;
        }
        else
        {
            fixtureDef.density = 1e3f;
            fixtureDef.friction = 0.1;
        }
        // FIXME bullet will still slip
        body->CreateFixture(&fixtureDef);

        mapping[ent] = body;
    });
    int velocityIterations = 2;
    int positionIterations = 2;
    b2d_world.Step(delta, velocityIterations, positionIterations);

    // fetch data
    world->view<Position, Velocity>().each([&](auto ent, auto &pos, auto &vel) {
        b2Body *body = mapping[ent];
        b2Vec2 position = body->GetPosition();
        b2Vec2 velocity = body->GetLinearVelocity();
        pos.from(position);
        vel.from(velocity);
        if (!world->has<BulletData>(ent))
            vel *= .93;
    });

    // Only movement
    // world->view<Position, Velocity>().each([&](auto ent, auto &pos, auto &vel) {
    //     pos += vel * delta;
    // });
}
