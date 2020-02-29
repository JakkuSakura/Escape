//
// Created by jack on 20-2-25.
//

#include "logic.h"
#include "application.h"
#include "psysics.h"
#include "lifespan.h"
#include "weapons.h"
#include "agent.h"
#include "serialization.h"
#include "control.h"
namespace Escape {
    Logic::Logic() {
        addSubSystem(new PhysicsSystem());
        addSubSystem(new LifespanSystem());
        addSubSystem(new BulletSystem());
        addSubSystem(new WeaponSystem());
        addSubSystem(new AgentSystem());
        addSubSystem(new ControlSystem());
    }
    void Logic::initialize() {
        findSystem<Application>()->timeserver->setRate(60);
        SerializationHelper reader("assets/map.json");
        reader.deserialize(*world);
    }

}
