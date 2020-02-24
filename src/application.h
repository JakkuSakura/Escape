#if !defined(APPLICATION_H)
#define APPLICATION_H
#include "engine/window.h"
#include "engine/system.h"
#include "engine/utils.h"
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
    virtual bool isRunning() = 0;
};

} // namespace Escape

#endif // APPLICATION_H
