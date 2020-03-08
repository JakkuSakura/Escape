#include "agent.h"
#include "config.h"
#include "serialization.h"
#include <nlohmann/json.hpp>
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

    entt::entity AgentSystem::getPlayer(World *world, int player_id) {
        auto ps = getPlayers(world, player_id);
        if (!ps.empty())
            return ps[0];
        else
            return entt::null;
    }

    std::vector<entt::entity> AgentSystem::getPlayers(World *world, int player_id) {
        std::vector<entt::entity> players;
        world->view<AgentData>().each([&](entt::entity ent, auto &control) {
            if (control.player == player_id) {
                players.push_back(ent);
            }
        });
        return players;
    }

    bool AgentSystem::query(Query &q) {
        if (q.resource == "/player")
        {
            if(q.method == "GET")
            {
                entt::entity player = getPlayer(getWorld(), 1);
                q.result = entt::to_integral(player);
                return true;
            }
        }
        if (q.resource == "/entity")
        {
            if(q.method == "GET")
            {
                if(getWorld()->valid((entt::entity)to_int(q.parameters)))
                    q.result = getEntityInfo(getWorld(), (entt::entity)to_int(q.parameters));
                else
                    q.result = {};
                return true;
            }
        }
        return false;
    }

    void AgentSystem::initialize() {
        findSystem<MapServer>()->addProvider(this);
    }

}
