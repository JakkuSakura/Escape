#if !defined(ECSCORE_H)
#define ECSCORE_H
#include <ECS.h>
#include "system.h"
#include "utils.h"
#include <boost/bimap.hpp>

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

class ComponentRegister : public boost::bimap<std::string, ECS::TypeIndex>
{
    ComponentRegister(){};

public:
    static ComponentRegister &getInstance();
    void registerPair(const std::string &str, ECS::TypeIndex info)
    {
        insert(value_type(str, info));
    }
    ECS::TypeIndex getTypeInfo(const std::string &str) const
    {
        return left.at(str);
    }
    std::string getName(ECS::TypeIndex info) const
    {
        return right.at(info);
    }
};
#define REGISTER(New) RUN( \
                            \
    ComponentRegister::getInstance().registerPair(#New, ECS::getTypeIndex<New>()); \
    )

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
