//
// Created by jack on 20-2-25.
//
#include "separate_application.h"
using namespace Escape;
Escape::SeparateApplication::SeparateApplication(Window *window, System *core) {
    this->window = window;
    this->core = core;
    addSubSystem(window);
    addSubSystem(core);
}

bool Escape::SeparateApplication::isRunning() {
    return window->isRunning();
}
