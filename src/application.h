#if !defined(APPLICATION_H)
#define APPLICATION_H
#include "window.h"
#include "system.h"
#include "utils.h"
namespace Escape
{
 
class Application : public System
{
public:
    Application()
    {
        is_running = true;
    }
    virtual void loop()
    {
        foreach ([](System *sys) { sys->initialize(); });
        clock_type last_frame = game_clock();
        while (isRunning())
        {
            clock_type now = game_clock();
            clock_type delta = now - last_frame;
            updateAll(delta);
            last_frame = now;
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
};

} // namespace Escape

#endif // APPLICATION_H
