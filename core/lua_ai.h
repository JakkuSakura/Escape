//
// Created by jack on 20-3-4.
//

#ifndef ESCAPE_LUA_AI_H
#define ESCAPE_LUA_AI_H

#include "config.h"
#include "ai_system.h"
#include "lua_script_util.h"
#include "config.h"

namespace Escape {
    class Agent_Lua : public AgentControl {
        sol::state lua;
        LuaScriptUtil converter = lua;
        ControlSystem *control;
        std::string file;
    public:
        Agent_Lua(std::string filename);

        void init(ControlSystem *c) override;

        void update(float delta) override;


    };

}
#endif //ESCAPE_LUA_AI_H
