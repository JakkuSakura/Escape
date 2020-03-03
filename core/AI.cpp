//
// Created by jack on 20-2-29.
//

#include "AI.h"
#include "control.h"
#include <cstdlib>
#include "lua_script.h"
#include <glm/glm.hpp>
#include "config.h"

namespace Escape {
    float random_angle() {
        return (float) M_PI * 2 * (float) rand() / RAND_MAX;
    }

    class AgentControl : public Controller {
        friend AISystem;
        entt::entity id;

        void setEntityID(entt::entity id_) {
            this->id = id_;
        }

    public:
        ControlSystem *control;

        void init(ControlSystem *c) override {
            control = c;
        }

        entt::entity getEntityID() {
            return id;
        }
    };

    class Agent_Random : public AgentControl {
    public:
        Agent_Random() {

        }


        void update(float delta) override {
            for (entt::entity ent : control->findPlayers(0)) {
                float angle = random_angle();
                control->dispatch(ent, ChangeWeapon(WeaponType::SMG));
                control->dispatch(ent, Impulse(cos(angle) * AGENT_IMPULSE, sin(angle) * AGENT_IMPULSE));
                control->dispatch(ent, Shooting(random_angle()));
            }
        }

    };

    class Agent_Smarter : public AgentControl {
    public:
        Agent_Smarter() {

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
                    control->dispatch(ent, Shooting(angle));
                    control->dispatch(ent, Impulse(cos(angle) * AGENT_IMPULSE * .8, sin(angle) * AGENT_IMPULSE * .8));

                } else {
                    float angle = random_angle();
                    control->dispatch(ent, Impulse(cos(angle) * AGENT_IMPULSE * .8, sin(angle) * AGENT_IMPULSE * .8));
                }
            }
        }

    };

    class Agent_Lua : public AgentControl {
        sol::state lua;
        ControlSystem *control;
        std::string file;
    public:
        Agent_Lua(std::string &&filename) : file(std::move(filename)) {
            lua.open_libraries(sol::lib::base);
            lua.open_libraries(sol::lib::io);
        }

        void init(ControlSystem *c) override {
            control = c;

            lua["get"] = [&](int ent) -> sol::table {
                return getEntity((entt::entity) ent);
            };
            lua["id"] = getEntityID();

            lua.script_file(file);
        }

        void update(float delta) override {
            lua.script("update()");
        }

        sol::table getEntity(entt::entity ent) {
            return Converter::toTable(control->toJSON(ent));
        }
    };

    void AISystem::initialize() {

        for (entt::entity ent = AISystem::allocate(); ent != entt::null; ent = AISystem::allocate()) {
            insert(ent, new Agent_Lua("/home/jack/IdeaProjects/Escape/maps/map1/simple_ai.lua"));
        }
    }

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
        for (auto[key, value] : AIs) {
            if (!getWorld()->valid(key)) {
                AIs.erase(key);
                contr->removeController(value);
                delete value;
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
