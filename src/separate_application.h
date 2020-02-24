#if !defined(SEPARATE_APPLICATION_H)
#define SEPARATE_APPLICATION_H
#include "application.h"
namespace Escape
{

class SeparateApplication : public Application
{
public:
    SeparateApplication(Window *window, System *core)
    {
        this->window = window;
        this->core = core;
        addSubSystem(window);
        addSubSystem(core);
    }
    virtual bool isRunning() override
    {
        return window->isRunning();
    }
    Window *getWindow() const
    {
        return window;
    }
    System *getCore() const
    {
        return core;
    }

private:
    Window *window;
    System *core;
};
} // namespace Escape

#endif // SEPARATE_APPLICATION_H
