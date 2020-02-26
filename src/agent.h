#if !defined(AGENT_H)
#define AGENT_H
#include "MyECS.h"
#include "weapons.h"
#include "components.h"
namespace Escape
{
class AgentSystem : public ECSSystem
{
    static int index;
public:

    static Entity getPlayer(World *world, int player_id) {
        Entity player = entt::null;
        world->view<AgentData>().each([&](Entity ent, auto &control) {
            if (control.player == 1) {
                player = ent;
            }
        });
        return player;
    }

    static Entity createAgent(World *world, const Position &pos, int player);
    Entity createAgent(const Position &pos);

    void update(clock_type delta) override;
};
} // namespace Escape

#endif // AGENT_H
