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

        virtual void init(ControlSystem *msg) {

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
        void dispatch(int agent_id, const T &action) {
            world->assign_or_replace<Message<T>>(findPlayer(agent_id), Message<T>(agent_id, action));
        }
        void update(clock_type delta) {
            for(Controller *c : control)
            {
                c->update(delta);
            }
        }
        virtual ~ControlSystem() {
            for(Controller *c: control)
            {
                delete c;
            }
        }
        entt::entity findPlayer(int player_id) {
            return AgentSystem::getPlayer(world, player_id);
        }
        template<typename ... T>
        auto get(int player_id) {
            return world->get<T ...>(findPlayer(player_id));
        }
        template<typename ... T>
        auto has(int player_id) {
            return world->has<T ...>(findPlayer(player_id));
        }
        template<typename ... T>
        auto any(int player_id) {
            return world->any<T ...>(findPlayer(player_id));
        }
    };

}

#endif //ESCAPE_CONTROL_H
