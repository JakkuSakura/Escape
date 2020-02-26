//
// Created by jack on 20-2-25.
//

#include "logic.h"
#include "read_map.h"

namespace Escape {
    class InitMap : public Escape::ECSSystem {
    public:
        InitMap();

        void initialize() override;
    };

    void InitMap::initialize() {
        ECSSystem::initialize();
        ReadMap reader("assets/map.json");
        reader.readMapFile(world);
        // Escape::Entity player = logic->agent_system->createAgent(Position(0, 0));
        // logic->world->assign<Control>(player, Control{player : 1});
        // logic->agent_system->createAgent(Position(-200, 200));
        // logic->agent_system->createAgent(Position(0, 200));
        // logic->agent_system->createAgent(Position(200, 200));
    }

    void Escape::Logic::initialize() {
        SystemManager::initialize();
        timeserver = findSystem<Application>()->timeserver;
        timeserver->setRate(60);
    }

    Escape::Logic::Logic() {
        addSubSystem(movement_system = new PhysicsSystem());
        addSubSystem(lifespan_system = new LifespanSystem());
        addSubSystem(bullet_system = new BulletSystem());
        addSubSystem(weapon_system = new WeaponSystem());
        addSubSystem(agent_system = new AgentSystem());
        addSubSystem(new InitMap());
    }

    InitMap::InitMap() {}

}
