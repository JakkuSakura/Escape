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

    class LuaScript {
        using json = nlohmann::json;
        sol::state lua;
    public:
        LuaScript() {
            lua.open_libraries(sol::lib::base);
        }
        sol::state &getState() {
            return lua;
        }
        void reset() {
            lua.globals().clear();
        }
        void doString(const std::string &s) {
            lua.script(s);
        }

        void doFile(const std::string &file) {
            lua.script_file(file);
        }

        void setArray(const std::string &name, const json &js) {
            assert(js.is_array());
            auto table = lua.create_table();

            for (int i = 0; i < js.size(); ++i) {
                if(js[i].is_number())
                    table[i + 1] = js[i].get<double>();
                else if (js[i].is_string())
                    table[i + 1] = js[i].get<std::string>();
                else if (js[i].is_null())
                    continue;
                else {
                    throw std::runtime_error("Unsupported type in json array");
                }
            }
            lua[name] = table;
        }

        auto getArray(const std::string &name) {
            auto table = lua.get<sol::table>(name);
            json js;
            if (table.valid()) {
                for (int i = 1, length = table.size(); i <= length; ++i) {
                    if (table[i].valid()) {
                        auto type = table[i].get_type();
                        if (type == sol::type::nil || type == sol::type::none)
                            js.push_back(nullptr);
                        else if (type == sol::type::number)
                            js.push_back(table[i].get<double>());
                        else if (type == sol::type::string)
                            js.push_back(table[i].get<std::string>());
                        else {
                            throw std::runtime_error("Unsupported type in table");
                        }
                    }

                }
            }
            return js;
        }
    };

}


#endif //LUA_SCRIPT_H
