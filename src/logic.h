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
#include "movement.h"
#include "serialization.h"
#include "agent.h"
#include <sstream>
#include "MyECS.h"

namespace Escape {
    class Logic : public SystemManager {

    public:
        using SystemManager::world;
        MovementSystem *movement_system;
        BulletSystem *bullet_system;
        WeaponSystem *weapon_system;
        LifespanSystem *lifespan_system;
        AgentSystem *agent_system;
        TimeServer *timeserver;

        Entity getPlayer();

        void initialize() override;

        Logic();

        void fire(Entity ent, float angle);

        void move(Entity ent, const Velocity &vel);

        void changeWeapon(Entity ent, WeaponType type);
    };

}

#endif //ESCAPE_LOGIC_H
