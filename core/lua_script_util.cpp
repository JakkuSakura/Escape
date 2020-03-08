//
// Created by jack on 20-3-2.
//

#include "lua_script_util.h"

namespace Escape {
    LuaScriptUtil::json LuaScriptUtil::toJSON(const sol::object &obj) {
        auto type = obj.get_type();
        if (type == sol::type::table) {
            json js;
            for (auto &[key, value] : obj.as<sol::table>()) {
                if (key.get_type() == sol::type::string) {
                    js[key.as<std::string>()] = toJSON(value);
                } else if (key.get_type() == sol::type::number) {
                    js.push_back(toJSON(value));
                } else {
                    throw std::runtime_error("Do not support this kind of key");
                }
            }
            return js;
        } else if (type == sol::type::nil || type == sol::type::none)
            return nullptr;
        else if (type == sol::type::number)
            return obj.as<double>();
        else if (type == sol::type::string)
            return obj.as<std::string>();
        else if (type == sol::type::table)
            return toJSON(obj.as<sol::table>());
        else if (type == sol::type::boolean)
            return obj.as<bool>();
        else {
            throw std::runtime_error("Unsupported value type");
        }

    }

    sol::object LuaScriptUtil::toLuaObject(const LuaScriptUtil::json &js) {
        if (js.is_array()) {
            sol::table table(state, sol::create);
            for (int i = 0; i < js.size(); ++i) {
                table.raw_set(i + 1, toLuaObject(js[i]));
            }
            return table;
        } else if (js.is_object()) {
            sol::table table(state, sol::create);
            for (auto &[key, value] : js.items()) {
                table.raw_set(key, toLuaObject(value));
            }
            return table;
        } else {
            if (js.is_string())
                return sol::make_object(state, js.get<std::string>());
            else if (js.is_null())
                return sol::make_object(state, nullptr);
            else if (js.is_object() || js.is_array())
                return toLuaObject(js);
            else if (js.is_number_integer())
                return sol::make_object(state, js.get<int>());
            else if (js.is_number_float())
                return sol::make_object(state, js.get<double>());
            else if (js.is_boolean())
                return sol::make_object(state, js.get<bool>());
            else {
                std::cerr << js << std::endl;
                throw std::runtime_error("Do not support this json type");
            }
        }
    }

    LuaScriptUtil::LuaScriptUtil(sol::state &state_) : state(state_) {}

}
