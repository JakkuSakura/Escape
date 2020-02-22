#if !defined(ECSCORE_H)
#define ECSCORE_H
#include "ECS.h"
#include "system.h"
namespace Escape
{

class Application;
class ECSCore : public System
{
    ECS::World *world = ECS::World::createWorld();
public:
    Core()
    {
    }
    void setApp(Application *app)
    {
        this->app = app;
    }
    Application *getApp()
    {
        return app;
    }
    ECS::World *getWorld()
    {
        return world;
    }
    virtual void update(float delta)
    {
        world->tick(delta);
    };
    virtual ~Core()
    {
        world->destroyWorld();
    }
};

} // namespace Escape

#endif // ECSCORE_H
