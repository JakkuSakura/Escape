//
// Created by jack on 20-3-9.
//
#include "map_script_system.h"

namespace Escape {
    void on_collision(MapScriptSystem *this_, Collision evt) {
        if (this_->getState()["on_collision"].valid()) {
            char buf[64];
            sprintf(buf, "on_collision(%u, %u);\n", entt::to_integral(evt.a), entt::to_integral(evt.b));
            this_->getState().script(buf);
        }
    }

    MapScriptSystem::MapScriptSystem() {
        state.open_libraries(sol::lib::base);
        state.open_libraries(sol::lib::io);
        state.open_libraries(sol::lib::math);
    }

    void MapScriptSystem::initialize() {
        state["get"] = [&](const std::string &resource, const sol::object &parameter) -> sol::object {
            auto json = findSystem<RESTSystem>()->query("GET", resource, converter.toJSON(parameter));
            return converter.toLuaObject(json);
        };
        state["post"] = [&](const std::string &resource, const sol::object &parameter) -> sol::object {
            auto json = findSystem<RESTSystem>()->query("POST", resource, converter.toJSON(parameter));
            return converter.toLuaObject(json);
        };
        findSystem<EventSystem>()->listen(&on_collision, this);
    }

    void MapScriptSystem::loadMapScript(const std::string &s) {
        state.script_file(s);
    }


    void MapScriptSystem::update(float delta) {
        if (state["update"].get_type() == sol::type::function) {
            state.script("update();");
        }
    }

    sol::state &MapScriptSystem::getState() {
        return state;
    }

}
