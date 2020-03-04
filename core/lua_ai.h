//
// Created by jack on 20-3-4.
//

#ifndef ESCAPE_LUA_AI_H
#define ESCAPE_LUA_AI_H

#include "config.h"
#include "ai_system.h"
#include "lua_script.h"
#include "config.h"

namespace Escape {
    class Agent_Lua : public AgentControl {
        sol::state lua;
        Converter converter = lua;
        ControlSystem *control;
        std::string file;
    public:
        Agent_Lua(std::string &&filename);

        void init(ControlSystem *c) override;

        void update(float delta) override;

        void submit(entt::entity ent, const sol::table &table);

        sol::table getEntityInfo(entt::entity ent);
    };

}
#endif //ESCAPE_LUA_AI_H
