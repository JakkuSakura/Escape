#if !defined(DISPLAY_H)
#define DISPLAY_H
#include <string>
#include <iostream>
#include "system.h"

namespace Escape
{

class Window : public System
{
protected:
    std::string title;
    int width, height;
    clock_type delta;

public:
    Window(const std::string &title, int width, int height)
    {
        this->title = title;
        this->width = width;
        this->height = height;
    };
    virtual void render() = 0;
    virtual void update(clock_type delta)
    {
        this->delta = delta;
        processInput();
        render();
        postProcess();
    }
    virtual void postProcess()
    {
    }
    virtual bool isRunning() { return true; };
    virtual void processInput()
    {
    }
    virtual ~Window()
    {
    }

    virtual void windowResized(int width, int height)
    {
        this->width = width;
        this->height = height;
    }
};

} // namespace Escape

#endif // DISPLAY_H
