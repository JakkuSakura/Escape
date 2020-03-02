//
// Created by jack on 20-2-29.
//

#include "AI.h"
#include "control.h"
#include <ctime>
#include <cstdlib>
#include "lua_script.h"
#include <glm/glm.hpp>

namespace Escape {
    float random_angle() {
        return (float) M_PI * 2 * (float) rand() / RAND_MAX;
    }

    class Agent_Random : public Controller {
        ControlSystem *control;
    public:
        Agent_Random() {
            srand(time(0));
        }

        void init(ControlSystem *c) override {
            control = c;
        }

        void update(float delta) override {
            for (entt::entity ent : control->findPlayers(0)) {
                float angle = random_angle();
                control->dispatch(ent, ChangeWeapon{WeaponType::SMG});
                control->dispatch(ent, Impulse(cos(angle) * 20, sin(angle) * 20));
                control->dispatch(ent, Shooting{.angle = random_angle()});
            }
        }

    };

    class Agent_Smarter : public Controller {
        ControlSystem *control;
    public:
        Agent_Smarter() {
            srand(time(0));
        }

        void init(ControlSystem *c) override {
            control = c;
        }

        void update(float delta) override {

            for (entt::entity ent : control->findPlayers(0)) {
                control->dispatch(ent, ChangeWeapon{WeaponType::SHOTGUN});
                auto pos = control->get<Position>(ent);
                auto players = control->filter([&](entt::entity ent, Position &p, AgentData &agt) -> bool {
                    return glm::distance2(as<glm::vec2>(pos), as<glm::vec2>(p)) < 20 * 20 && agt.player != 0;
                });
                if (players.size()) {
                    auto p = control->get<Position>(players[0]);
                    float angle = atan2f(p.y - pos.y, p.x - pos.x);
                    control->dispatch(ent, Shooting{.angle =  angle});
                    control->dispatch(ent, Impulse(cos(angle) * 12, sin(angle) * 12));

                } else {
                    float angle = random_angle();
                    control->dispatch(ent, Impulse(cos(angle) * 12, sin(angle) * 12));
                }
            }
        }

    };

    class Agent_Lua : public Controller {
        LuaScript lua;
        ControlSystem *control;
    public:
        Agent_Lua(const std::string &file) {
            lua.doFile(file);
        }

        void init(ControlSystem *c) override {
            control = c;
        }

        void update(float delta) override {
            // TODO this has to be considered before implementing
        }

    };

    void AISystem::initialize() {
        auto contr = findSystem<ControlSystem>();
        contr->addController(new Agent_Smarter());
    }

    AISystem::AISystem() {

    }

}
