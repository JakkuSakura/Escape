#include "agent.h"
#include "config.h"
#include "serialization.h"
#include <nlohmann/json.hpp>

namespace Escape {
    entt::entity
    AgentSystem::createAgent(World *world, const Position &pos, int player, int group, const std::string &ai) {
        assert(world != nullptr);
        entt::entity agent = world->create();
        world->assign<Name>(agent, "agent");
        world->assign<Position>(agent, pos);
        world->assign<AgentData>(agent, AgentData{player, group, ai});
        world->assign<Health>(agent, 100);
        world->assign<CircleShape>(agent, AGENT_RADIUS);
        world->assign<Velocity>(agent);
        world->assign<Collidable>(agent, true);
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
        if (q.resource == "/player") {
            if (q.method == "GET") {
                entt::entity player = getPlayer(getWorld(), 1);
                if (player != entt::null)
                    q.result = entt::to_integral(player);
                else
                    q.result = nullptr;
                return true;
            }
        }
        if (q.resource == "/agents") {
            if (q.method == "GET") {
                getWorld()->view<AgentData>().each([&](entt::entity ent, auto &data) {
                    if (!q.parameters.is_null()) {
                        if (q.parameters.count("player") && data.player != q.parameters["player"])
                            return;
                        if (q.parameters.count("group") && data.group != q.parameters["group"])
                            return;
                    }
                    q.result.push_back(ent);
                });
                return true;
            } else if (q.method == "POST") {
                q.result = createAgent(getWorld(), Position(q.parameters["x"], q.parameters["y"]),
                                       q.parameters["player"],
                                       q.parameters["group"], q.parameters["ai"]);
                return true;
            }
        }
        if (q.resource == "/entity") {
            if (q.method == "GET") {
                if (q.parameters.is_null()) {
                    q.result = nullptr;
                    return true;
                }

                auto entity_id = (entt::entity) q.parameters.get<ENTT_ID_TYPE>();
                if (getWorld()->valid(entity_id))
                    q.result = getEntityInfo(getWorld(), entity_id);
                else
                    q.result = nullptr;
                return true;
            } else if (q.method == "POST") {
                auto entity_id = (entt::entity) q.parameters["entity"].get<ENTT_ID_TYPE>();
                setEntityInfo(getWorld(), entity_id, q.parameters["components"]);
                return true;
            }
        }
        if (q.resource == "/entities") {
            if (q.method == "GET") {
                getWorld()->each([&](entt::entity ent) {
                    q.result.push_back(ent);
                });
                return true;
            }

        }
        return false;
    }

    void AgentSystem::initialize() {
        findSystem<RESTSystem>()->addProvider(this);
    }

}
