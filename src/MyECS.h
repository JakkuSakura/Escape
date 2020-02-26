#if !defined(ECSCORE_H)
#define ECSCORE_H
#include "engine/system.h"
#include "engine/utils.h"
#include <entt/entity/registry.hpp>
namespace Escape
{
using Entity = entt::entity;
using World = entt::registry;
class SystemManager : public System
{
protected:
    World *world = new World();
public:
    SystemManager()
    {
    }
    World *getWorld()
    {
        return world;
    }
    virtual void update(clock_type delta)
    {
        // TODO some update
    };
    virtual ~SystemManager()
    {
        delete world;
    }
};

class ECSSystem : public System
{
public:
    World *world;
    SystemManager *system_manager;
    void initialize() override
    {
        system_manager = findSystem<SystemManager>();
        world = system_manager->getWorld();
    }
};

} // namespace Escape

#endif // ECSCORE_H
