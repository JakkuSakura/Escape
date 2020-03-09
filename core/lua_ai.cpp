//
// Created by jack on 20-3-4.
//

#include "lua_ai.h"
#include "restful_system.h"

namespace Escape {

    Agent_Lua::Agent_Lua(std::string filename) : file(std::move(filename)) {
        lua.open_libraries(sol::lib::base);
        lua.open_libraries(sol::lib::io);
        lua.open_libraries(sol::lib::math);
    }

    void Agent_Lua::init(ControlSystem *c) {
        control = c;

        lua["id"] = getEntityID();
        lua["get"] = [&](const std::string &resource, const sol::object &parameter) -> sol::object {
            auto json = control->query("GET", resource, converter.toJSON(parameter));
            return converter.toLuaObject(json);
        };
        lua["control"] = [&](const sol::table &tab) {
            control->dispatch(getEntityID(), converter.toJSON(tab));
        };

        lua.script_file(file);
    }

    void Agent_Lua::update(float delta) {
        lua.script("update()");
    }


}
