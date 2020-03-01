#include "agent.h"

namespace Escape {
    entt::entity AgentSystem::createAgent(World *world, const Position &pos, int player) {
        assert(world != nullptr);
        entt::entity agent = world->create();
        world->assign<Name>(agent, "agent");
        world->assign<Position>(agent, pos);
        world->assign<AgentData>(agent, AgentData{.player =  player});
        world->assign<Health>(agent, 100);
        world->assign<Hitbox>(agent, Hitbox{.radius =  1});
        world->assign<Velocity>(agent);
        world->assign<Weapon>(agent, Weapon{WeaponType::HANDGUN, 0});
        return agent;

    }

    entt::entity AgentSystem::createAgent(const Position &pos) {
        return createAgent(world, pos, 0);
    }

    void AgentSystem::update(clock_type delta) {
        world->view<Health>().each([&](entt::entity ent, auto &health) {
            if (health.health <= 0)
                world->destroy(ent);
        });
    }

}
