#include "../Classes/AppDelegate.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "../Classes/Settings.h"

USING_NS_CC;

int main(int argc, char **argv)
{
    Escape::settings["map"] = argv[1];
    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
