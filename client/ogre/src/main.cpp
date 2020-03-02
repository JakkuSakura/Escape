
#include "logic.h"
#include "display.h"
#include "map_converter.h"
using namespace Escape;

int main(int argc, const char **argv) {
    if(argc < 2)
    {
        std::cerr << "You must specify a map folder" << std::endl;
        exit(-1);
    }
    auto display = new DisplayOgre();
    MapConverter mapConverter;
    auto *world = mapConverter.convert(argv[1]);
    System system;
    Logic *logic = new Logic();
    logic->setWorld(world);
    system.addSubSystem(logic);
    system.addSubSystem(display);
    system.foreach([](System *sys){
        sys->initialize();
    });
    while (display->isRunning()) {
        system.updateAll(1 / 60.0f);
    }
    return 0;
}