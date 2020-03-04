#if !defined(AGENT_H)
#define AGENT_H

#include "MyECS.h"
#include "weapon_system.h"
#include "components.h"
#include <vector>

namespace Escape {

    class AgentSystem : public ECSSystem {
    public:

        static inline entt::entity getPlayer(World *world, int player_id) {
            auto ps = getPlayers(world, player_id);
            if (!ps.empty())
                return ps[0];
            else
                return entt::null;
        }

        static inline std::vector<entt::entity> getPlayers(World *world, int player_id) {
            std::vector<entt::entity> players;
            world->view<AgentData>().each([&](entt::entity ent, auto &control) {
                if (control.player == player_id) {
                    players.push_back(ent);
                }
            });
            return players;
        }

        void update(float delta) override;

        static entt::entity createAgent(World *world, const Position &pos, int player, int group = 0, std::string &&ai = "basic");
    };
} // namespace Escape

#endif // AGENT_H
