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
    template<typename T>
    void getProperty(json &obj, const std::string &key, T &value, bool &success) {
        for (auto &e : obj["properties"]) {
            if (e["name"] == key) {
                success = true;
                value = e["value"];
                return;
            }
        }
        success = false;
        return;
    }

    World *MapConverter::convert() {

        entt::registry *world = new entt::registry();
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
                        int type = data[j * width + i];
                        if (type > 0) {
                            if (configuration["tiles"][type - 1]["type"] == "Wall") {
                                TerrainSystem::createWall(world, x, y, tilewidth * scale_x, tileheight * scale_y);
                            } else if (configuration["tiles"][type - 1]["type"] == "Water") {
                                TerrainSystem::createWater(world, x, y, tilewidth * scale_x, tileheight * scale_y);
                            } else if (configuration["tiles"][type - 1]["type"] == "Barrier") {
                                TerrainSystem::createBarrier(world, x, y, tilewidth * scale_x, tileheight * scale_y);
                            }
                        }
                    }
                }
            } else if (layer["objects"].is_array()) {
                for (auto &&obj : layer["objects"]) {
                    if (obj["type"] == "SpawnPoint") {
                        float x = ((float) obj["x"]) * scale_x, y = -((float) obj["y"]) * scale_y;
                        if (obj["name"] == "player") {
                            AgentSystem::createAgent(world, Position(x, y), 1, 1);
                        } else if (obj["name"] == "agent") {
                            std::string ai_file = "simple_ai";
                            bool success;
                            getProperty(obj, "ai", ai_file, success);
                            AgentSystem::createAgent(world, Position(x, y), 0, 0, std::move(ai_file));
                        }
                    }
                }
            }
        }
        return world;
    }

    MapConverter::MapConverter(const std::string &input) {
        {
            std::ifstream is(input + "/configuration.json");
            is >> configuration;
        }

        {
            std::ifstream is(input + "/mapdata.json");
            is >> map;
        }
    }

    void MapConverter::coordinate(const std::string &obj, std::string &filename, int &x1, int &y1, int &x2, int &y2) {
        int tilewidth = configuration["tilewidth"], tileheight = configuration["tileheight"];
        int imagewidth = configuration["imagewidth"], imageheight = configuration["imageheight"];
        int margin = configuration["margin"], spacing = configuration["spacing"];
        int col = (imagewidth - margin * 2 + spacing) / (tilewidth + spacing)
        , row = (imageheight - margin * 2 + spacing) / (tileheight + spacing);

        for (int i = 0; i < configuration["tiles"].size(); ++i) {
            if (configuration["tiles"][i]["type"] == obj) {
                filename = configuration["image"];
                int id = configuration["tiles"][i]["id"];
                int c = id % col, r = id / col;

                x1 = margin - spacing + c * (tilewidth + spacing);
                y1 = margin - spacing + r * (tileheight + spacing);

                x2 = x1 + spacing + tilewidth;
                y2 = y1 + spacing + tileheight;
                return;
            }
        }
    }
} // namespace Escape
