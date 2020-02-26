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
    SystemManager();
    World *getWorld();
    ~SystemManager() override;
};

class ECSSystem : public System
{
public:
    World *world = nullptr;
    void configure() override;
};

} // namespace Escape

#endif // ECSCORE_H
