//
// Created by jack on 20-2-29.
//

#include "AI.h"
#include "control.h"
#include <ctime>
#include <cstdlib>
namespace Escape {
    float random_angle() {
        return (float)M_PI * 2 * (float)rand() / RAND_MAX;
    }
    class Agent_1 : public Controller {
        ControlSystem *control;
    public:
        Agent_1() {
            srand(time(0));
        }
        void init(ControlSystem *c) override {
            control = c;
        }
        void update(clock_type delta) override {
            for(entt::entity ent : control->findPlayers(0)) {
                float angle = random_angle();
                control->dispatch(ent, ChangeWeapon{WeaponType::SMG});
                control->dispatch(ent, Impulse(cos(angle) * 20, sin(angle) * 20));
                control->dispatch(ent, Shooting{.angle = random_angle()});
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
