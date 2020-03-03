//
// Created by jack on 20-2-29.
//

#ifndef ESCAPE_CONTROL_H
#define ESCAPE_CONTROL_H

#include "MyECS.h"
#include "components.h"
#include "agent.h"
#include <vector>
#include "event_system.h"

namespace Escape {
    class ControlSystem;

    class Controller {
    public:
        virtual ~Controller() {

        }

        virtual void init(ControlSystem *control) {

        }

        virtual void update(float delta) {

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
        void dispatch(entt::entity entity, T action) {
            action.actor = entity;
            findSystem<EventSystem>()->enqueue(action);
        }

        void update(float delta) {
            for (Controller *c : control) {
                c->update(delta);
            }
        }

        virtual ~ControlSystem() {
            for (Controller *c: control) {
                delete c;
            }
        }

        template<typename ... Args>
        std::vector<entt::entity> filter(std::function<bool(entt::entity, Args ...)> filter_) {
            std::vector<entt::entity> results;
            getWorld()->view<std::remove_reference_t<Args> ...>().each(
                    [&](auto ent, Args ... args) {
                        if (filter_(ent, args ...))
                            results.push_back(ent);
                    });
            return results;
        }

        template<typename Fn>
        std::vector<entt::entity> filter(Fn filter_) {
            return filter(std::function(filter_));
        }


        entt::entity findPlayer(int player_id) {
            return AgentSystem::getPlayer(getWorld(), player_id);
        }

        auto findPlayers(int player_id) {
            return AgentSystem::getPlayers(getWorld(), player_id);
        }

        template<typename ... T>
        auto get(entt::entity ent) {
            return getWorld()->get<T ...>(ent);
        }

        template<typename ... T>
        auto has(entt::entity ent) {
            return getWorld()->has<T ...>(ent);
        }

        template<typename ... T>
        auto any(entt::entity ent) {
            return getWorld()->any<T ...>(ent);
        }
    };

}

#endif //ESCAPE_CONTROL_H
