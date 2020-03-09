//
// Created by jack on 20-3-2.
//

#ifndef ESCAPE_MAP_SCRIPT_SYSTEM_H
#define ESCAPE_MAP_SCRIPT_SYSTEM_H

#include "lua_script_util.h"
#include <sol/sol.hpp>

namespace Escape {
    class MapScriptSystem : public ECSSystem {
        sol::state state;
        LuaScriptUtil converter = state;
    public:
        MapScriptSystem() {
            state.open_libraries(sol::lib::base);
            state.open_libraries(sol::lib::io);
            state.open_libraries(sol::lib::math);

            state["get"] = [&](const std::string &resource, const sol::object &parameter) -> sol::object {
                auto json = findSystem<RESTSystem>()->query("GET", resource, converter.toJSON(parameter));
                return converter.toLuaObject(json);
            };
            state["post"] = [&](const std::string &resource, const sol::object &parameter) -> sol::object {
                auto json = findSystem<RESTSystem>()->query("POST", resource, converter.toJSON(parameter));
                return converter.toLuaObject(json);
            };
        }

        void loadMapScript(const std::string &s) {
            state.script_file(s);
        }

        void update(float delta) {
            if (state["update"].get_type() == sol::type::function) {
                state.script("update();");
            }
        }
    };
}
#endif //ESCAPE_MAP_SCRIPT_SYSTEM_H
