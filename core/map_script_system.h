//
// Created by jack on 20-3-2.
//

#ifndef ESCAPE_MAP_SCRIPT_SYSTEM_H
#define ESCAPE_MAP_SCRIPT_SYSTEM_H

#include "lua_script_util.h"
namespace Escape {
    // For map
    class LuaScriptSystem : public ECSSystem {
        LuaScript script;
    public:
        LuaScriptSystem() {

        }

        void loadMapScript(std::string &&s) {
            script.reset();
            script.doFile(s);
        }

        void update(float delta) {
            if (script.getState()["update"].valid())
                script.doString("update();");
        }
    };
}
#endif //ESCAPE_MAP_SCRIPT_SYSTEM_H
