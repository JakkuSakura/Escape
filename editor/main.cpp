#include "map_converter.h"
#include <iostream>
int main(int argc, const char **argv) {
    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " input_map_directory" << std::endl;
        exit(1);
    }
    Escape::MapConverter converter;
    converter.convert(argv[1]);
    return 0;
}