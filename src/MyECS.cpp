#include "MyECS.h"
namespace Escape
{
    void ECSSystem::configure() {
        auto *sysMgr = findSystem<SystemManager>(false);
        if(sysMgr)
            world = sysMgr->getWorld();
    }

    SystemManager::~SystemManager() {
        delete world;
    }

    World *SystemManager::getWorld() {
        return world;
    }

    SystemManager::SystemManager() {
    }
} // namespace Escape
