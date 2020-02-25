#include "movement.h"
void Escape::MovementSystem::move(Entity ent, const glm::vec2 &speed) {
    world->assign_or_replace<Velocity>(ent, speed.x, speed.y);
}

void Escape::MovementSystem::update(clock_type delta) {
    world->view<Velocity, Position>().each([&](auto Ent, auto &vel, auto &pos){
        pos += vel * delta;
    });
}
