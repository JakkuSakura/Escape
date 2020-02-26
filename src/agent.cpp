#include "agent.h"

int Escape::AgentSystem::index = 0;

Escape::Entity Escape::AgentSystem::createAgent(Escape::World *world, const Escape::Position &pos, int player) {
    assert(world != nullptr);
    Entity agent = world->create();
    world->assign<Name>(agent, "agent");
    world->assign<Position>(agent, pos);
    world->assign<AgentData>(agent, AgentData{.id =  ++index, .player =  player});
    world->assign<Health>(agent, 100);
    world->assign<Hitbox>(agent, Hitbox{.radius =  1});
    world->assign<Velocity>(agent);
    world->assign<Weapon>(agent, Weapon{WeaponType::HANDGUN, 0});
    return agent;

}

Escape::Entity Escape::AgentSystem::createAgent(const Escape::Position &pos) {
    return createAgent(world, pos, 0);
}

void Escape::AgentSystem::update(clock_type delta) {
    world->view<Health>().each([&](Entity ent, auto &health){
        if(health.health <= 0)
            world->destroy(ent);
    });
}
