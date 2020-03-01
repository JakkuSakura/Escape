#include "map_converter.h"
#include <iostream>
int main(int argc, const char **argv) {
    if(argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " input_map.json output_map.json" << std::endl;
        exit(1);
    }
    Escape::MapConverter converter;
    converter.convert(argv[1], argv[2]);
    return 0;
}