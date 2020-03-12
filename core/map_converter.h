//
// Created by jack on 20-3-1.
//

#ifndef ESCAPE_MAP_CONVERTER_H
#define ESCAPE_MAP_CONVERTER_H

#include <string>
#include "MyECS.h"
#include <nlohmann/json.hpp>
namespace Escape
{
class MapConverter {
    nlohmann::json configuration;
    nlohmann::json map;
public:
    std::string tileset;
    MapConverter(const std::string &input);
    World *convert();
    void coordinate(int id, int &x1, int &y1, int &x2, int &y2);

    int tilewidth, tileheight;
    int imagewidth, imageheight;
    int margin, spacing;
    int col, row;
};
    
} // namespace Escape



#endif //ESCAPE_MAP_CONVERTER_H
