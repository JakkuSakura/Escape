#if !defined(APPLICATION_H)
#define APPLICATION_H
#include "window.h"
#include "system.h"
#include "utils.h"
#include "timeserver.h"
namespace Escape
{
 
class Application : public System
{
public:
    TimeServer *timeserver;
    Application()
    {
        addSubSystem(timeserver = new TimeServer(30));
        is_running = true;
    }
    virtual void loop()
    {
        foreach ([](System *sys) { sys->initialize(); });
        while (isRunning())
        {
            updateAll(timeserver->getDelta());
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
