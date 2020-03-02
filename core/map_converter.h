//
// Created by jack on 20-3-1.
//

#ifndef ESCAPE_MAP_CONVERTER_H
#define ESCAPE_MAP_CONVERTER_H

#include <string>
#include "MyECS.h"

namespace Escape
{
class MapConverter {
public:
    World *convert(const std::string &input);
};
    
} // namespace Escape



#endif //ESCAPE_MAP_CONVERTER_H
