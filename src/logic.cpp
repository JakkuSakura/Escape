//
// Created by jack on 20-2-25.
//

#include "logic.h"
#include "read_map.h"
namespace Escape {
    class InitMap : public Escape::ECSSystem {
        Escape::Logic *logic;

    public:
        InitMap(Escape::Logic *logic);

        void initialize() override;
    };

    void InitMap::initialize() {
        ReadMap reader("assets/map.json");
        reader.readMapFile(logic->world);
        // Escape::Entity player = logic->agent_system->createAgent(Position(0, 0));
        // logic->world->assign<Control>(player, Control{player : 1});
        // logic->agent_system->createAgent(Position(-200, 200));
        // logic->agent_system->createAgent(Position(0, 200));
        // logic->agent_system->createAgent(Position(200, 200));
    }

    Escape::Entity Escape::Logic::getPlayer() {
        Entity player = entt::null;
        world->view<AgentData>().each([&](Entity ent, auto &control) {
            if (control.player == 1) {
                player = ent;
            }
        });
        return player;
    }

    void Escape::Logic::initialize() {
        SystemManager::initialize();
        timeserver = findSystem<Application>()->timeserver;
        timeserver->setRate(60);
    }

    Escape::Logic::Logic() {
        addSubSystem(movement_system = new MovementSystem());
        addSubSystem(lifespan_system = new LifespanSystem());
        addSubSystem(bullet_system = new BulletSystem());
        addSubSystem(weapon_system = new WeaponSystem());
        addSubSystem(agent_system = new AgentSystem());
        addSubSystem(new InitMap(this));
    }

    void Escape::Logic::fire(Escape::Entity ent, float angle) {
        weapon_system->fire(ent, angle);
    }

    void Escape::Logic::move(Escape::Entity ent, const Escape::Velocity &vel) {
        movement_system->move(ent, vel);
    }

    void Escape::Logic::changeWeapon(Escape::Entity ent, Escape::WeaponType type) {
        // FIXME By changing weapon quickly, the player has a change of shooting each frame
        if (world->has<Weapon>(ent)) {
            auto &w = world->get<Weapon>(ent);
            if (type != w.weapon)
                world->assign_or_replace<Weapon>(ent, Weapon{type, 0, 0});
        } else {
            world->assign<Weapon>(ent, Weapon{type, 0});
        }
    }

    InitMap::InitMap(Escape::Logic *logic) : logic(logic) {}

}
