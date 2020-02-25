#if !defined(ECSCORE_H)
#define ECSCORE_H
#include <ECS.h>
#include "engine/system.h"
#include "engine/utils.h"
// #include <boost/bimap.hpp>
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

#define REGISTER(New)

#define COMPONENT_AS(New, Old) \
    NEW_TYPE(New, Old);        \
    REGISTER(New)

#define COMPONENT_NEW(New, df) \
    struct New                 \
    {                          \
        df                     \
    };                         \
    REGISTER(New)

} // namespace Escape

#endif // ECSCORE_H
