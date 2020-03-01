
#include "logic.h"
#include "display.h"
#include "timeserver.h"

using namespace Escape;

int main() {
    auto display = new DisplayOgre();
    System system;
    system.addSubSystem(new Logic());
    system.addSubSystem(display);
    system.foreach([](System *sys){
        sys->initialize();
    });
    while (display->isRunning()) {
        system.updateAll(1 / 60.0f);
    }
    return 0;
}