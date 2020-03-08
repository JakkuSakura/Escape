//
// Created by jack on 20-2-29.
//

#ifndef ESCAPE_CONTROL_H
#define ESCAPE_CONTROL_H

#include "MyECS.h"
#include "components.h"
#include "agent.h"
#include <vector>
#include <sstream>
#include <set>
#include "event_system.h"
#include "serialization.h"
#include "nlohmann/json.hpp"

namespace Escape {
    class ControlSystem;

    class Controller {
        friend ControlSystem;

    public:
        virtual ~Controller() {

        }

        virtual void init(ControlSystem *control) {

        }


        virtual void update(float delta) {

        }
    };

    class ControlSystem : public ECSSystem {
        std::set<Controller *> control;
    public:
        ControlSystem() {

        }

        void addController(Controller *c) {
            control.insert(c);
            c->init(this);
        }

        // this will return the ownership
        void removeController(Controller *c) {
            control.erase(control.find(c));
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

        void dispatch(entt::entity ent, const nlohmann::json &event) {
            auto &type = event["type"];
            if (type == "shooting")
                dispatch(ent, Shooting(event["angle"]));
            else if (type == "change_weapon") {
                SerializationHelper helper;
                std::stringstream ss;
                ss << event["weapon"];
                WeaponType weaponType;
                helper.deserialize(weaponType, ss);
                dispatch(ent, ChangeWeapon(weaponType));
            } else if (type == "move") {
                dispatch(ent, Impulse(event["x"], event["y"]));
            }
        }

        bool valid(entt::entity ent) {
            return getWorld()->valid(ent);
        }
    };

}

#endif //ESCAPE_CONTROL_H
