#if !defined(WINDOW_OGRE_H)
#define WINDOW_OGRE_H
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include "engine/window.h"
#include <cassert>
#include "engine/system.h"
#include "engine/utils.h"
namespace Escape
{
class InputState : public OgreBites::InputListener
{
public:
    static const int MAX_KEY_NUM = 4096;
    bool keys[MAX_KEY_NUM] = {0};
    float mouse_x = 0, mouse_y = 0;
    bool mouse[4]={0};

    virtual void frameRendered(const Ogre::FrameEvent &evt);
    virtual bool keyPressed(const OgreBites::KeyboardEvent &evt);
    virtual bool keyReleased(const OgreBites::KeyboardEvent &evt);
    virtual bool touchMoved(const OgreBites::TouchFingerEvent &evt);
    virtual bool touchPressed(const OgreBites::TouchFingerEvent &evt);
    virtual bool touchReleased(const OgreBites::TouchFingerEvent &evt);
    virtual bool mouseMoved(const OgreBites::MouseMotionEvent &evt);
    virtual bool mouseWheelRolled(const OgreBites::MouseWheelEvent &evt);
    virtual bool mousePressed(const OgreBites::MouseButtonEvent &evt);
    virtual bool mouseReleased(const OgreBites::MouseButtonEvent &evt);
    virtual bool textInput(const OgreBites::TextInputEvent &evt);
};
class WindowOgre : public Window, public OgreBites::ApplicationContext
{
protected:
    clock_type delta;
    bool running;
    Ogre::Root *root;
    Ogre::SceneManager *scnMgr;
    Ogre::SceneNode *camNode;
    Ogre::Camera *cam;
    bool closed = false;

public:
    InputState input;

    WindowOgre(const std::string &title, int width, int height);;
    virtual void processInput();
    virtual void update(clock_type delta);
    virtual void render();
    virtual void postProcess() override;
    void stop();
    virtual bool isRunning();
    virtual ~WindowOgre();
    virtual void windowResized(Ogre::RenderWindow *rw) override;
    virtual void windowResized(int width, int height) override;
    void setup() final override;
};
} // namespace Escape

#endif // WINDOW_OGRE_H
