//
// Created by jack on 20-2-25.
//

#include "logic.h"
#include "psysics.h"
#include "lifespan.h"
#include "weapon_system.h"
#include "agent.h"
#include "control.h"
#include "ai_system.h"
#include "event_system.h"
#include "restful_system.h"
#include "map_script_system.h"
namespace Escape {
    void Logic::addSystems() {
        addSubSystem(new RESTSystem());
        addSubSystem(new MapScriptSystem());
        addSubSystem(new TimeServer(60));
        addSubSystem(new PhysicsSystem());
        addSubSystem(new LifespanSystem());
        addSubSystem(new BulletSystem());
        addSubSystem(new WeaponSystem());
        addSubSystem(new AISystem());
        addSubSystem(new ControlSystem());
        addSubSystem(new AgentSystem());
        addSubSystem(new EventSystem());
    }
    Logic::Logic() {
        addSystems();
    }
    Logic::Logic(World *wo) {
        addSystems();
        setWorld(wo);
    }

    void Logic::setWorld(World *wo) {
        delete this->world;
        this->world = wo;
    }

}
