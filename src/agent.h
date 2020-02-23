#if !defined(AGENT_H)
#define AGENT_H
#include "engine/MyECS.h"
#include "weapons.h"
namespace Escape
{
class AgentSystem : public ECSSystem
{
public:
    Entity *createAgent(const Position &pos)
    {
        assert(world != nullptr);
        Entity *agent = world->create();
        agent->assign<Name>("agent");
        agent->assign<Position>(pos);
        agent->assign<Health>(100);
        agent->assign<Hitbox>(Hitbox{radius: 16});
        agent->assign<Weapon>(Weapon{WeaponType::SHOTGUN, 0});
        return agent;
    }

    void update(clock_type delta) override
    {
        world->each<Health>([&](Entity *ent, ComponentHandle<Health> health){
            if(health->health <= 0)
                world->destroy(ent, false);
        });
    }
};
} // namespace Escape

#endif // AGENT_H
