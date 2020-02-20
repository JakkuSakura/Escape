#if !defined(APPLICATION_H)
#define APPLICATION_H
#include "window.h"
#include "core.h"
#include <ctime>
namespace Escape
{
class Application
{
public:
    Application(Window *window, Core *core)
    {
        this->window = window;
        this->core = core;
        running = true;
        last_frame = clock() - (1000.0 / 60.0);
    }
    virtual void loop()
    {
        clock_t now = clock();
        float delta = (now - last_frame) / 1000.0f;
        while (running)
        {
            window->update(delta);
            core->update(delta);
            checkRunning();
        }
    }
    virtual ~Application()
    {
        delete window;
        delete core;
    }
    virtual void checkRunning()
    {
        running = window->isRunning();
    }
    Window *getWindow() const
    {
        return window;
    }
    Core *getCore() const
    {
        return core;
    }

private:
    Window *window;
    Core *core;
    bool running;
    clock_t last_frame;
};

} // namespace Escape

#endif // APPLICATION_H
