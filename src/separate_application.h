#if !defined(SEPARATE_APPLICATION_H)
#define SEPARATE_APPLICATION_H
#include "application.h"
namespace Escape
{

class SeparateApplication : public Application
{
public:
    SeparateApplication(Window *window, System *core);
    virtual bool isRunning() override;
    Window *getWindow() const;
    System *getCore() const;

private:
    Window *window;
    System *core;
};
} // namespace Escape

#endif // SEPARATE_APPLICATION_H
