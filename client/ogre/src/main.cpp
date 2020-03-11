#include "my_escape.h"
#include "display.h"

using namespace Escape;

int main(int argc, const char **argv) {
    if (argc < 2) {
        std::cerr << "You must specify a map folder" << std::endl;
        exit(-1);
    }

    auto display = new DisplayOgre();

    MyEscape system(argv[1]);
    system.addSubSystem(display);

    system.foreach([](System *sys) {
        sys->initialize();
    });
    while (display->isRunning()) {
        system.updateAll(1 / 60.0f);
    }
    return 0;
}