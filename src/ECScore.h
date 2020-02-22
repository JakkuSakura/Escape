#if !defined(ECSCORE_H)
#define ECSCORE_H
#include "ECS.h"
#include "system.h"
namespace Escape
{

class ECSCore : public System
{
    ECS::World *world = ECS::World::createWorld();
public:
    ECSCore()
    {
    }
    ECS::World *getWorld()
    {
        return world;
    }
    virtual void update(float delta)
    {
        world->tick(delta);
    };
    virtual ~ECSCore()
    {
        world->destroyWorld();
    }
};

} // namespace Escape

#endif // ECSCORE_H
