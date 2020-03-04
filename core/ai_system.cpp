//
// Created by jack on 20-2-29.
//

#include "ai_system.h"
#include "control.h"

namespace Escape {

    AISystem::AISystem() {

    }

    AISystem::~AISystem() {
    }

    entt::entity AISystem::allocate() {
        auto agents = getWorld()->view<AgentData>();
        for (entt::entity ent : agents) {
            auto data = agents.get<AgentData>(ent);
            if (data.player == 0 && AIs.count(ent) == 0) {
                return ent;
            }
        }
        return entt::null;
    }

    void AISystem::update(float delta) {
        auto contr = findSystem<ControlSystem>();
        auto iter = AIs.begin();
        while (iter != AIs.end()) {
            if (!getWorld()->valid(iter->first)) {
                contr->removeController(iter->second);
                delete iter->second;
                AIs.erase(iter++);
            } else {
                ++iter;
            }
        }
    }

    void AISystem::insert(entt::entity ent, AgentControl *agt) {
        assert(AIs.count(ent) == 0);
        std::cerr << "Enabled AI for entity " << entt::to_integral(ent) << std::endl;
        AIs[ent] = agt;
        agt->setEntityID(ent);
        auto contr = findSystem<ControlSystem>();
        contr->addController(agt);
    }


}
