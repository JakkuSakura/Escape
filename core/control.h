//
// Created by jack on 20-2-29.
//

#ifndef ESCAPE_CONTROL_H
#define ESCAPE_CONTROL_H

#include "MyECS.h"
#include "components.h"
#include "agent.h"
#include <vector>
#include "message.h"

namespace Escape {
    class ControlSystem;

    class Controller {
    public:
        virtual ~Controller() {

        }

        virtual void init(ControlSystem *control) {

        }

        virtual void update(clock_type delta) {

        }
    };

    class ControlSystem : public ECSSystem {
        std::vector<Controller *> control;
    public:
        ControlSystem() {

        }

        void addController(Controller *c) {
            c->init(this);
            control.push_back(c);
        }

        template<typename T>
        void dispatch(entt::entity entity, const T &action) {
            world->assign_or_replace<Message<T>>(entity, Message<T>(entity, action));
        }

        void update(clock_type delta) {
            for (Controller *c : control) {
                c->update(delta);
            }
        }

        virtual ~ControlSystem() {
            for (Controller *c: control) {
                delete c;
            }
        }

        entt::entity findPlayer(int player_id) {
            return AgentSystem::getPlayer(world, player_id);
        }

        auto findPlayers(int player_id) {
            return AgentSystem::getPlayers(world, player_id);
        }

        template<typename ... T>
        auto get(entt::entity ent) {
            return world->get<T ...>(ent);
        }

        template<typename ... T>
        auto has(entt::entity ent) {
            return world->has<T ...>(ent);
        }

        template<typename ... T>
        auto any(entt::entity ent) {
            return world->any<T ...>(ent);
        }
    };

}

#endif //ESCAPE_CONTROL_H
