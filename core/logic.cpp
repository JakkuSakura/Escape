//
// Created by jack on 20-2-25.
//

#include "logic.h"
#include "psysics.h"
#include "lifespan.h"
#include "weapons.h"
#include "agent.h"
#include "serialization.h"
#include "control.h"
#include "AI.h"
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
    }
    void Logic::initialize() {
//        SerializationHelper reader("assets/map.json");
//        reader.deserialize(*world);
//        TODO add map file support
    }

}
