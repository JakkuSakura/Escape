//
// Created by jack on 20-3-4.
//

#include "lua_ai.h"
namespace Escape {

    Agent_Lua::Agent_Lua(std::string &&filename) : file(std::move(filename)) {
        lua.open_libraries(sol::lib::base);
        lua.open_libraries(sol::lib::io);
        lua.open_libraries(sol::lib::math);
    }

    void Agent_Lua::init(ControlSystem *c) {
        control = c;

        lua["get"] = [&](const sol::object &query) -> sol::object {

            if (query.get_type() == sol::type::number) {
                entt::entity ent = (entt::entity) query.as<ENTT_ID_TYPE>();
                if (!control->valid(ent))
                    return sol::nil;
                return getEntityInfo(ent);
            } else if (query.get_type() == sol::type::string && query.as<std::string>() == "player") {
                entt::entity player = control->findPlayer(1);
                if (player == entt::null) {
                    return sol::nil;
                }
                return converter.toTable(control->getEntityInfo(player));
            }
            throw std::runtime_error("Canot find the command " + query.as<std::string>());
        };
        lua["id"] = getEntityID();
        lua["post"] = [&](const sol::table &tab) {
            submit(getEntityID(), tab);
        };

        lua.script_file(file);
    }

    void Agent_Lua::update(float delta) {
        lua.script("update()");
    }

    void Agent_Lua::submit(entt::entity ent, const sol::table &table) {

        control->dispatch(ent, converter.toJSON(table));
    }

    sol::table Agent_Lua::getEntityInfo(entt::entity ent) {
        return converter.toTable(control->getEntityInfo(ent));
    }
}
