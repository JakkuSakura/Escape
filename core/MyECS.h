#if !defined(ECSCORE_H)
#define ECSCORE_H

#include "engine/system.h"
#include "engine/utils.h"
#include <entt/entity/registry.hpp>

namespace Escape {
    using World = entt::registry;

    class SystemManager : public System {
    protected:
        World *world = new World();
    public:
        SystemManager() {}

        World *getWorld() {
            return world;
        }

        ~SystemManager() override {
            delete world;
        }
    };

    class ECSSystem : public System {
        SystemManager *sysmgr;
    public:
        void configure() override {
            sysmgr = findSystem<SystemManager>(true, false);
        }
        void unconfigure() override {
            sysmgr = nullptr;
        };
    protected:
        World *getWorld() {
            assert(sysmgr);
            return sysmgr->getWorld();
        }
    };

} // namespace Escape

#endif // ECSCORE_H
