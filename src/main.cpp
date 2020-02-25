
#include "logic.h"
#include "display.h"

int main()
{
    Escape::SeparateApplication app(new Escape::DisplayOgre(), new Escape::Logic());
    app.loop();
    return 0;
}