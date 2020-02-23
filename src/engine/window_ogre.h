#if !defined(WINDOW_OGRE_H)
#define WINDOW_OGRE_H
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include "window.h"
#include <cassert>
#include "system.h"
#include "utils.h"
namespace Escape
{
namespace Render
{
class InputState : public OgreBites::InputListener
{
public:
    bool keys[5000] = {0};
    float mouse_x, mouse_y;
    bool mouse[4];

    virtual void frameRendered(const Ogre::FrameEvent &evt) {}
    virtual bool keyPressed(const OgreBites::KeyboardEvent &evt)
    {
        keys[evt.keysym.sym] = true;
        return false;
    }
    virtual bool keyReleased(const OgreBites::KeyboardEvent &evt)
    {
        keys[evt.keysym.sym] = false;
        return false;
    }
    virtual bool touchMoved(const OgreBites::TouchFingerEvent &evt) { return false; }
    virtual bool touchPressed(const OgreBites::TouchFingerEvent &evt) { return false; }
    virtual bool touchReleased(const OgreBites::TouchFingerEvent &evt) { return false; }
    virtual bool mouseMoved(const OgreBites::MouseMotionEvent &evt)
    {
        return false;
    }
    virtual bool mouseWheelRolled(const OgreBites::MouseWheelEvent &evt) { return false; }
    virtual bool mousePressed(const OgreBites::MouseButtonEvent &evt)
    {

        mouse_x = evt.x;
        mouse_y = evt.y;
        mouse[evt.button] = true;
        return false;
    }
    virtual bool mouseReleased(const OgreBites::MouseButtonEvent &evt)
    {
        mouse_x = evt.x;
        mouse_y = evt.y;
        mouse[evt.button] = false;
        return false;
    }
    virtual bool textInput(const OgreBites::TextInputEvent &evt) { return false; }
};
class WindowOGRE : public Window, public OgreBites::ApplicationContext
{
protected:
    clock_type delta;
    bool running;
    Ogre::Root *root;
    Ogre::SceneManager *scnMgr;
    bool closed = false;

public:
    InputState input;

    WindowOGRE(const std::string &title, int width, int height) : Window(title, width, height), OgreBites::ApplicationContext(title)
    {
        running = true;
        initApp();
        addInputListener(&input);
    };
    virtual void processInput()
    {
        if (input.keys[OgreBites::SDLK_ESCAPE])
            stop();
    }
    virtual void render()
    {
        root->renderOneFrame();
    }
    virtual void postProcess() override
    {
    }
    void stop()
    {
        closed = true;
    }
    virtual bool isRunning()
    {
        return !closed && !root->endRenderingQueued();
    }
    virtual ~WindowOGRE()
    {
        closeApp();
    }

    virtual void windowResized(int width, int height)
    {
        this->width = width;
        this->height = height;
    }
    void setup() final override
    {
        OgreBites::ApplicationContext::setup();
        root = getRoot();
        scnMgr = root->createSceneManager();
        Ogre::RTShader::ShaderGenerator *shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
        shadergen->addSceneManager(scnMgr);

        Ogre::Light *light = scnMgr->createLight("MainLight");
        Ogre::SceneNode *lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
        lightNode->setPosition(0, 10, 15);
        lightNode->attachObject(light);

        Ogre::SceneNode *camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
        camNode->setPosition(0, 0, 15);
        camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

        Ogre::Camera *cam = scnMgr->createCamera("myCam");
        cam->setNearClipDistance(5); // specific to this sample
        cam->setAutoAspectRatio(true);
        camNode->attachObject(cam);

        // and tell it to render into the main window
        getRenderWindow()->addViewport(cam);
    }


};
} // namespace Render

} // namespace Escape

#endif // WINDOW_OGRE_H
