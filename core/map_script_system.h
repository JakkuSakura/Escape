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
        MapScriptSystem();
        sol::state &getState();
        void initialize() override;

        void loadMapScript(const std::string &s);
        void update(float delta);
    };
}
#endif //ESCAPE_MAP_SCRIPT_SYSTEM_H
