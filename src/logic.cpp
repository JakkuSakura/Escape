//
// Created by jack on 20-2-25.
//

#include "logic.h"
#include "read_map.h"
#include "application.h"
#include "psysics.h"
#include "lifespan.h"
#include "weapons.h"
#include "agent.h"

namespace Escape {

    void Escape::Logic::initialize() {
        findSystem<Application>()->timeserver->setRate(60);
        addSubSystem(new PhysicsSystem());
        addSubSystem(new LifespanSystem());
        addSubSystem(new BulletSystem());
        addSubSystem(new WeaponSystem());
        addSubSystem(new AgentSystem());
        ReadMap reader("assets/map.json");
        reader.readMapFile(world);
    }

}
