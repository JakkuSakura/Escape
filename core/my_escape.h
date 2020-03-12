//
// Created by jack on 20-3-11.
//

#ifndef ESCAPE_MY_ESCAPE_H
#define ESCAPE_MY_ESCAPE_H

#include "ai_system.h"
#include "lua_ai.h"
#include "logic.h"
#include "map_converter.h"
#include "map_script_system.h"
namespace Escape {

    class MyEscape : public ECSSystem {
        std::string mapfile, mapscript;
    public:
        MyEscape(std::string map) : mapfile(std::move(map)), mapConverter(mapfile), mapscript(mapfile + "/game.lua") {
            auto *world = mapConverter.convert();
            auto *logic = new Logic(world);
            addSubSystem(logic);
            findSystem<MapScriptSystem>()->loadMapScript( mapscript);
            ECSSystem::configure();
        }

        void update(float delta) override {
            auto ai_system = findSystem<AISystem>();
            for (entt::entity ent = ai_system->allocate(); ent != entt::null; ent = ai_system->allocate()) {
                auto *world = getWorld();
                auto data = world->get<AgentData>(ent);
                auto ai_path = mapfile + "/" + data.ai + ".lua";
                ai_system->insert(ent, new Agent_Lua(std::move(ai_path)));
            }
        }

        using ECSSystem::getWorld;
        MapConverter mapConverter;
    };
}
#endif //ESCAPE_MY_ESCAPE_H
