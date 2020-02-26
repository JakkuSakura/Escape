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
        findSystem<Application>()->timeserver->setRate(60);
        
    }

    Escape::Logic::Logic() {
        addSubSystem(new PhysicsSystem());
        addSubSystem(new LifespanSystem());
        addSubSystem(new BulletSystem());
        addSubSystem(new WeaponSystem());
        addSubSystem(new AgentSystem());
        addSubSystem(new InitMap());
    }

    InitMap::InitMap() {}

}
