#if !defined(CORE_H)
#define CORE_H
#include "ECS.h"
namespace Escape
{

class Application;
class Core
{
    Application *app = nullptr;
    ECS::World* world = ECS::World::createWorld();
public:
    void setApp(Application *app)
    {
        this->app = app;
    }
    Application *getApp()
    {
        return app;
    }
    ECS::World *getWorld() {
        return world;
    }
    virtual void update(float delta){
        world->tick(delta);
    };
    virtual ~Core() {
        world->destroyWorld();
    }
};

} // namespace Escape

#endif // CORE_H
