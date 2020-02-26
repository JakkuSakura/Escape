
#include "logic.h"
#include "display.h"
#include "timeserver.h"
using namespace Escape;
int main()
{
    SeparateApplication app(new DisplayOgre(), new Logic());
    app.addSubSystem(new FPSCounter());
    app.loop();
    return 0;
}