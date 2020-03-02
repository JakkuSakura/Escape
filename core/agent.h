#if !defined(AGENT_H)
#define AGENT_H

#include "MyECS.h"
#include "weapons.h"
#include "components.h"
#include <vector>

namespace Escape {

    class AgentSystem : public ECSSystem {
    public:

        static inline entt::entity getPlayer(World *world, int player_id) {
            auto ps = getPlayers(world, player_id);
            if (ps.size())
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

        static entt::entity createAgent(World *world, const Position &pos, int player);

        entt::entity createAgent(const Position &pos);

        void update(float delta) override;
    };
} // namespace Escape

#endif // AGENT_H
