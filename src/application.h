#if !defined(APPLICATION_H)
#define APPLICATION_H
#include "window.h"
#include "system.h"
#include <ctime>
namespace Escape
{
class Application : public System
{
public:
    Application()
    {
        is_running = true;
        last_frame = clock() - (1000.0 / 60.0);
    }
    virtual void loop()
    {
        foreach ([](System *sys) { sys->initialize(); });
        clock_t now = clock();
        float delta = (now - last_frame) / 1000.0f;
        while (isRunning())
        {
            updateAll(delta);
        }
    }
    virtual ~Application()
    {
    }
    virtual bool isRunning()
    {
        return is_running;
    }
    void stop()
    {
        is_running = false;
    }

protected:
    bool is_running;
    clock_t last_frame;
};

} // namespace Escape

#endif // APPLICATION_H
