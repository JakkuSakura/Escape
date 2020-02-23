#if !defined(ECSCORE_H)
#define ECSCORE_H
#include "ECS.h"
#include "system.h"
namespace Escape
{
using ECS::ComponentHandle;
using ECS::Entity;
using ECS::World;
class SystemManager : public System
{
    World *world = World::createWorld();

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
    World *world;
    SystemManager *system_manager;
    void initialize() override
    {
        system_manager = findSystem<SystemManager>();
        world = system_manager->getWorld();
    }
};
struct Name : public std::string
{
    using std::string::string;
};

} // namespace Escape

#endif // ECSCORE_H
