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
    float delta;

public:
    Window(const std::string &title, int width, int height);;
    virtual void render() = 0;
    virtual void update(float delta);
    virtual void postProcess();
    virtual bool isRunning() { return true; };
    virtual void processInput();
    virtual ~Window();

    virtual void windowResized(int width, int height);
};

} // namespace Escape

#endif // DISPLAY_H
