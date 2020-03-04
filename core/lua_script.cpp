//
// Created by jack on 20-3-2.
//

#include "lua_script.h"

namespace Escape {
    Converter::json Converter::toJSON(const sol::table &table) {
        json js;
        for (auto &[key, value] : table) {
            auto type = value.get_type();

            if (key.get_type() == sol::type::string) {
                if (type == sol::type::nil || type == sol::type::none)
                    js[key.as<std::string>()] = nullptr;
                else if (type == sol::type::number)
                    js[key.as<std::string>()] = value.as<double>();
                else if (type == sol::type::string)
                    js[key.as<std::string>()] = value.as<std::string>();
                else if (type == sol::type::table)
                    js[key.as<std::string>()] = toJSON(value.as<sol::table>());
                else if (type == sol::type::boolean)
                    js[key.as<std::string>()] = value.as<bool>();
                else {
                    throw std::runtime_error("Unsupported value type in table");
                }
            } else if (key.get_type() == sol::type::number) {
                if (type == sol::type::nil || type == sol::type::none)
                    js.push_back(nullptr);
                else if (type == sol::type::number)
                    js.push_back(value.as<double>());
                else if (type == sol::type::string)
                    js.push_back(value.as<std::string>());
                else if (type == sol::type::table)
                    js.push_back(toJSON(value.as<sol::table>()));
                else if (type == sol::type::boolean)
                    js.push_back(value.as<bool>());
                else {
                    throw std::runtime_error("Unsupported value type in table");
                }

            } else {
                throw std::runtime_error("Do not support this kind of key");
            }

        }
        return js;
    }

    sol::table Converter::toTable(const Converter::json &js) {
        sol::table table(state, sol::create);

        if (js.is_array()) {
            for (int i = 0; i < js.size(); ++i) {
                if (js[i].is_string())
                    table.raw_set(i + 1, js[i].get<std::string>());
                else if (js[i].is_null())
                    table.raw_set(i + 1, nullptr);
                else if (js[i].is_object() || js[i].is_array())
                    table.raw_set(i + 1, toTable(js[i]));
                else if (js[i].is_number())
                    table.raw_set(i + 1, js[i].get<double>());
                else if (js[i].is_boolean())
                    table.raw_set(i + 1, js[i].get<bool>());
                else {
                    std::cerr << js[i] << std::endl;
                    throw std::runtime_error("Do not support this json type");
                }

            }
        } else if (js.is_object()) {
            for (auto &[key, value] : js.items()) {
                if (value.is_string())
                    table.raw_set(key, value.get<std::string>());
                else if (value.is_null())
                    table.raw_set(key, nullptr);
                else if (value.is_object() || value.is_array())
                    table.raw_set(key, toTable(value));
                else if (value.is_number())
                    table.raw_set(key, value.get<double>());
                else if (value.is_boolean())
                    table.raw_set(key, value.get<bool>());
                else {
                    std::cerr << value << std::endl;
                    throw std::runtime_error("Do not support this json type");
                }
            }
        } else {
            throw std::runtime_error("Do not support json type other than array and object");
        }
        return table;
    }

    Converter::Converter(sol::state &state_) : state(state_) {}

}
