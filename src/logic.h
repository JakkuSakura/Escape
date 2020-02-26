//
// Created by jack on 20-2-25.
//

#ifndef ESCAPE_LOGIC_H
#define ESCAPE_LOGIC_H
#include <iostream>
#include <fstream>
#include "separate_application.h"
#include "MyECS.h"
#include "engine/utils.h"
#include "weapons.h"
#include "psysics.h"
#include "serialization.h"
#include "agent.h"
#include <sstream>
#include "MyECS.h"

namespace Escape {
    class Logic : public SystemManager {

    public:
        using SystemManager::world;
        PhysicsSystem *movement_system;
        BulletSystem *bullet_system;
        WeaponSystem *weapon_system;
        LifespanSystem *lifespan_system;
        AgentSystem *agent_system;
        TimeServer *timeserver;

        void initialize() override;

        Logic();

    };

}

#endif //ESCAPE_LOGIC_H
