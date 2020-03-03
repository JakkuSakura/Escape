//
// Created by jack on 20-2-25.
//

#include "logic.h"
#include "psysics.h"
#include "lifespan.h"
#include "weapons.h"
#include "agent.h"
#include "control.h"
#include "AI.h"
#include "event_system.h"
namespace Escape {
    Logic::Logic() {
        addSubSystem(new TimeServer(60));
        addSubSystem(new PhysicsSystem());
        addSubSystem(new LifespanSystem());
        addSubSystem(new BulletSystem());
        addSubSystem(new WeaponSystem());
        addSubSystem(new AgentSystem());
        addSubSystem(new ControlSystem());
        addSubSystem(new AISystem());
        addSubSystem(new EventSystem());
    }

    void Logic::setWorld(World *wo) {
        delete this->world;
        this->world = wo;
    }

}
