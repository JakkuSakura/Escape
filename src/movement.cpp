#include "movement.h"
#include <Box2D/Box2D.h>
#include <map>
Escape::MovementSystem::MovementSystem()
{
}
void Escape::MovementSystem::move(Entity ent, const glm::vec2 &speed)
{
    world->assign_or_replace<Velocity>(ent, speed.x, speed.y);
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
            b2Body *wall = b2d_world.CreateBody(&wallDef);
            b2PolygonShape wallBox;
            wallBox.SetAsBox(ter.arguments[0] / 2, ter.arguments[1] / 2);
            wall->CreateFixture(&wallBox, 0.0f);
            mapping[ent] = wall;
        }
    });

    // put stuff in box2d
    world->view<Position, Velocity>().each([&](auto ent, auto &pos, auto &vel) {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(pos.x, pos.y);
        b2Body *body = b2d_world.CreateBody(&bodyDef);
        b2FixtureDef fixtureDef;
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(1.0f, 1.0f);
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        body->CreateFixture(&fixtureDef);

        mapping[ent] = body;
    });
    int velocityIterations = 6;
    int positionIterations = 2;
    b2d_world.Step(delta, velocityIterations, positionIterations);

    // fetch data
    world->view<Position, Velocity>().each([&](auto ent, auto &pos, auto &vel) {
        b2Body *body = mapping[ent];
        b2Vec2 position = body->GetPosition();
        b2Vec2 velocity = body->GetLinearVelocity();
        pos.from(position);
        vel.from(velocity);

    });

    // Only movement
    // world->view<Position, Velocity>().each([&](auto ent, auto &pos, auto &vel) {
    //     pos += vel * delta;
    // });
}
