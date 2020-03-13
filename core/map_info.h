//
// Created by jack on 20-3-13.
//

#ifndef ESCAPE_MAP_INFO_H
#define ESCAPE_MAP_INFO_H

#include "MyECS.h"
#include "components.h"
#include "restful_system.h"

namespace Escape {

    class MapInfoSystem : public ECSSystem, public ResourceProvider {
        entt::entity mapinfo = entt::null;

    public:
        void initialize() override {
            mapinfo = getWorld()->create();
            getWorld()->assign<MapInfo>(mapinfo);
            findSystem<RESTSystem>()->addProvider(this);
        }

        bool query(Query &q) override {
            if (q.resource == "/map_info") {
                if (q.method == "GET") {
                    auto &info = getWorld()->get<MapInfo>(mapinfo);
                    if (info.count(q.parameters.get<std::string>()))
                        q.result = info.at(q.parameters.get<std::string>());
                    else
                        q.result = nullptr;
                    return true;
                } else if (q.method == "POST") {
                    auto &info = getWorld()->get<MapInfo>(mapinfo);
                    info[q.parameters["key"]] = q.parameters["value"];
                    return true;
                }
            }
            return false;
        }
    };

}


#endif //ESCAPE_MAP_INFO_H
