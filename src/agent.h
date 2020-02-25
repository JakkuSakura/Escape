#if !defined(AGENT_H)
#define AGENT_H
#include "MyECS.h"
#include "weapons.h"
#include "components.h"
namespace Escape
{
class AgentSystem : public ECSSystem
{
    size_t index = 0;
public:
    Entity createAgent(const Position &pos)
    {
        assert(world != nullptr);
        Entity agent = world->create();
        world->assign<Name>(agent, "agent");
        world->assign<Position>(agent, pos);
        world->assign<ID>(agent, ID{++index});
        world->assign<Health>(agent, 100);
        world->assign<Hitbox>(agent, Hitbox{radius: 16});
        world->assign<Weapon>(agent, Weapon{WeaponType::HANDGUN, 0});
        return agent;
    }

    void update(clock_type delta) override
    {
        world->view<Health>().each([&](Entity ent, auto &health){
            if(health.health <= 0)
                world->destroy(ent);
        });
    }
};
} // namespace Escape

#endif // AGENT_H
