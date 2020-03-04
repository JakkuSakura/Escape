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
    class Converter {
        using json = nlohmann::json;
        sol::state &state;
    public:
        Converter(sol::state &state_);
        json toJSON(const sol::table &table);

        sol::table toTable(const json &js);

    };

}


#endif //LUA_SCRIPT_H
