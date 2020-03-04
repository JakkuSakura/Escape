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
namespace Escape {
    Logic::Logic() {
        addSubSystem(new TimeServer(60));
        addSubSystem(new PhysicsSystem());
        addSubSystem(new LifespanSystem());
        addSubSystem(new BulletSystem());
        addSubSystem(new WeaponSystem());
        addSubSystem(new AgentSystem());
        addSubSystem(new AISystem());
        addSubSystem(new ControlSystem());
        addSubSystem(new EventSystem());
    }

    void Logic::setWorld(World *wo) {
        delete this->world;
        this->world = wo;
    }

}
