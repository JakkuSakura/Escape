#if !defined(CORE_H)
#define CORE_H
namespace Escape
{
class Application;
class Core
{
    Application *app = nullptr;

public:
    void setApp(Application *app)
    {
        this->app = app;
    }
    Application *getApp()
    {
        return app;
    }
    virtual void update(float delta){};
    virtual ~Core() {}
};

} // namespace Escape

#endif // CORE_H
