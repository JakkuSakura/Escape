#if !defined(ECSCORE_H)
#define ECSCORE_H
#include "ECS.h"
#include "system.h"
namespace Escape
{

class SystemManager : public System
{
    ECS::World *world = ECS::World::createWorld();
public:
    SystemManager()
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
    virtual ~SystemManager()
    {
        world->destroyWorld();
    }
};

class ECSSystem : public System
{
public:
    ECS::World *world;
    SystemManager *system_manager;
    void initialize() override
    {
        system_manager = findSystem<SystemManager>();
        assert(system_manager != nullptr);
        world = system_manager->getWorld();
    }
};

} // namespace Escape

#endif // ECSCORE_H
