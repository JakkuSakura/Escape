#include "agent.h"
#include "config.h"

namespace Escape {
    entt::entity AgentSystem::createAgent(World *world, const Position &pos, int player, int group, std::string &&ai) {
        assert(world != nullptr);
        entt::entity agent = world->create();
        world->assign<Name>(agent, "agent");
        world->assign<Position>(agent, pos);
        world->assign<AgentData>(agent, AgentData{player, group, std::move(ai)});
        world->assign<Health>(agent, 100);
        world->assign<Hitbox>(agent, Hitbox{.radius =  AGENT_RADIUS});
        world->assign<Velocity>(agent);
        world->assign<Weapon>(agent, Weapon{WeaponType::HANDGUN, 0});
        return agent;

    }

    void AgentSystem::update(float delta) {
        getWorld()->view<Health>().each([&](entt::entity ent, auto &health) {
            if (health.health <= 0)
                getWorld()->destroy(ent);
        });
    }

}
