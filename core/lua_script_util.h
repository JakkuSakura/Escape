//
// Created by jack on 20-3-2.
//

#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include <string>
#include <iostream>
#include <lua.hpp>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include "MyECS.h"
#include "control.h"

namespace Escape {
    class LuaScriptUtil {
        using json = nlohmann::json;
        sol::state &state;
    public:
        LuaScriptUtil(sol::state &state_);
        json toJSON(const sol::object &obj);

        sol::object toLuaObject(const json &js);

    };

}


#endif //LUA_SCRIPT_H
