//
// Created by jack on 20-3-1.
//

#include "map_converter.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "terrain.h"
#include "agent.h"

using nlohmann::json;
namespace Escape {
    World *MapConverter::convert(const std::string &input) {

        json configuration;
        {
            std::ifstream is(input + "/configuration.json");
            is >> configuration;
        }

        json map;
        {
            std::ifstream is(input + "/mapdata.json");
            is >> map;
        }

        entt::registry *world = new entt::registry ;
        int width = map["width"], height = map["height"];
        float tilewidth = map["tilewidth"], tileheight = map["tileheight"];
        float scale_x = 1.0f / 16.0f, scale_y = 1.0f / 16.0f;
        for (auto &&layer : map["layers"]) {
            if (layer["data"].is_array()) {
                auto &&data = layer["data"];
                for (size_t i = 0; i < height; i++) {
                    for (size_t j = 0; j < width; j++) {
                        float x = (i * tilewidth + (float) layer["x"]) * scale_x, y =
                                -(j * tileheight + (float) layer["y"]) * scale_y;
                        int type = data[i * width + j];
                        if(type > 0)
                        {
                            if (configuration["tiles"][type - 1]["type"] == "Wall")
                            {              // It's wall
                                TerrainSystem::createWall(world, x, y, tilewidth * scale_x, tileheight * scale_y);
                            }
                        }
                    }
                }
            } else if (layer["objects"].is_array()) {
                for (auto &&obj : layer["objects"]) {
                    if (obj["type"] == "SpawnPoint") {
                        float x = ((float) obj["x"]) * scale_x, y = -((float) obj["y"]) * scale_y;
                        if (obj["name"] == "player") {
                            AgentSystem::createAgent(world, Position(x, y), 1);
                        } else if (obj["name"] == "agent") {
                            AgentSystem::createAgent(world, Position(x, y), 0);
                        }
                    }
                }
            }
        }
        return world;
    }
} // namespace Escape
