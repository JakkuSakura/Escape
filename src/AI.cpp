//
// Created by jack on 20-2-29.
//

#include "AI.h"
#include "control.h"
namespace Escape {
    class Agent_1 : public Controller {
        ControlSystem *control;
    public:
        Agent_1() {

        }
        void init(ControlSystem *c) override {
            control = c;
        }
        void update(clock_type delta) override {
            for(entt::entity ent : control->findPlayers(0)) {
//                control->
            }
        }

    };
    void AISystem::initialize() {
        auto contr = findSystem<ControlSystem>();
        contr->addController(new Agent_1());
    }

    AISystem::AISystem() {

    }

}
