//
// Created by jack on 20-3-2.
//

#ifndef ESCAPE_RESTFUL_SYSTEM_H
#define ESCAPE_RESTFUL_SYSTEM_H

#include "engine/system.h"
#include "entt/entity/registry.hpp"
#include "serialization.h"
#include <string>
#include <utility>

namespace Escape {
    class Query {
    public:
        std::string method;
        std::string resource;
        nlohmann::json parameters;
        nlohmann::json result;

        Query(std::string method_, std::string resource_, nlohmann::json parameters_)
                : method(std::move(method_)),
                  resource(std::move(resource_)),
                  parameters(std::move(parameters_)),
                  result() {
        }
    };

    class ResourceProvider {
    public:
        // method resource parameters
        virtual bool query(Query &q) = 0;
    };

    /**
     * REST System provides RESTful API for a map, for the use of both scripting system and c++ system.
     * It also supports commond line for scripting system.
     */
    class RESTSystem : public System {
        entt::registry registry;
        std::vector<ResourceProvider *> providers;
    public:
        RESTSystem() {
        }

        void initialize() override {

        }

        nlohmann::json query(const std::string &method, const std::string &resource, const nlohmann::json &parameters) {
            Query qu(method, resource, parameters);
            for (auto &provider : providers) {
                if (provider->query(qu))
                    return qu.result;
            }
            return qu.result;
        }

        void addProvider(ResourceProvider *p) {
            providers.push_back(p);
        }
    };

}

#endif //ESCAPE_RESTFUL_SYSTEM_H
