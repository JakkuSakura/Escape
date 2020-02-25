//
// Created by jack on 20-2-25.
//

#ifndef ESCAPE_LOGIC_H
#define ESCAPE_LOGIC_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "separate_application.h"
#include "engine/window_ogre.h"
#include <OgreRenderSystem.h>
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
        class PostInit : public ECSSystem {
            Logic *logic;

        public:
            PostInit(Logic *logic) : logic(logic) {}

            void initialize() override {
                Entity player = logic->agent_system->createAgent(Position(0, 0));
                logic->world->assign<Control>(player, Control{player : 1});
                logic->agent_system->createAgent(Position(-200, 200));
                logic->agent_system->createAgent(Position(0, 200));
                logic->agent_system->createAgent(Position(200, 200));
            }
        };

    public:
        MovementSystem *movement_system;
        BulletSystem *bullet_system;
        WeaponSystem *weapon_system;
        LifespanSystem *lifespan_system;
        AgentSystem *agent_system;
        TimeServer *timeserver;

        Entity getPlayer() {
            Entity player = entt::null;
            world->view<Control>().each([&](Entity ent, auto &control) {
                if (control.player == 1) {
                    player = ent;
                }
            });
            assert(player != entt::null);
            return player;
        }

        void initialize() override {
            SystemManager::initialize();
            timeserver = findSystem<Application>()->timeserver;
            timeserver->setRate(60);
        }

        Logic() {
            addSubSystem(movement_system = new MovementSystem());
            addSubSystem(lifespan_system = new LifespanSystem());
            addSubSystem(bullet_system = new BulletSystem());
            addSubSystem(weapon_system = new WeaponSystem());
            addSubSystem(agent_system = new AgentSystem());
            addSubSystem(new PostInit(this));
        }

        void fire(Entity ent, float angle) {
            weapon_system->fire(ent, angle);
        }

        void move(Entity ent, const Velocity &vel) {
            movement_system->move(ent, vel);
        }

        void changeWeapon(Entity ent, WeaponType type) {
            // FIXME By changing weapon quickly, the player has a change of shooting each frame
            if (world->has<Weapon>(ent)) {
                auto &w = world->get<Weapon>(ent);
                if (type != w.weapon)
                    world->assign_or_replace<Weapon>(ent, Weapon{type, 0, 0});
            } else {
                world->assign<Weapon>(ent, Weapon{type, 0});
            }
        }
    };

}

#endif //ESCAPE_LOGIC_H
