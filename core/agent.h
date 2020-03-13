#if !defined(AGENT_H)
#define AGENT_H

#include "MyECS.h"
#include "weapon_system.h"
#include "components.h"
#include <vector>
#include "restful_system.h"

namespace Escape {

    class AgentSystem : public ECSSystem, public ResourceProvider {
    public:

        static entt::entity getPlayer(World *world, int player_id);

        static std::vector<entt::entity> getPlayers(World *world, int player_id);

        void update(float delta) override;

        bool query(Query &q) override;

        void initialize() override;

        static entt::entity
        createAgent(World *world, const Position &pos, int player, int group = 0, const std::string &ai = "");
    };
} // namespace Escape

#endif // AGENT_H
