//
// Created by jack on 20-2-25.
//
#include "window_ogre.h"
using namespace Escape;

bool InputState::keyPressed(const OgreBites::KeyboardEvent &evt) {
    if(evt.keysym.sym < MAX_KEY_NUM)
        keys[evt.keysym.sym] = true;
    else {
        // std::cerr << "Cannot handle key down " << evt.keysym.sym << std::endl;
    }
    return false;
}

void InputState::frameRendered(const Ogre::FrameEvent &evt) {}

bool InputState::keyReleased(const OgreBites::KeyboardEvent &evt) {
    if(evt.keysym.sym < MAX_KEY_NUM)
        keys[evt.keysym.sym] = false;
    else {
        // std::cerr << "Cannot handle key up " << evt.keysym.sym << std::endl;
    }
    return false;
}

bool InputState::touchMoved(const OgreBites::TouchFingerEvent &evt) { return false; }

bool InputState::touchPressed(const OgreBites::TouchFingerEvent &evt) { return false; }

bool InputState::touchReleased(const OgreBites::TouchFingerEvent &evt) { return false; }

bool InputState::mouseMoved(const OgreBites::MouseMotionEvent &evt) {
    mouse_x = evt.x;
    mouse_y = evt.y;
    return false;
}

bool InputState::mouseWheelRolled(const OgreBites::MouseWheelEvent &evt) { return false; }

bool InputState::mousePressed(const OgreBites::MouseButtonEvent &evt) {
    mouse_x = evt.x;
    mouse_y = evt.y;
    mouse[evt.button] = true;
    return false;
}

bool InputState::mouseReleased(const OgreBites::MouseButtonEvent &evt) {
    mouse_x = evt.x;
    mouse_y = evt.y;
    mouse[evt.button] = false;
    return false;
}

bool InputState::textInput(const OgreBites::TextInputEvent &evt) { return false; }

WindowOgre::WindowOgre(const std::string &title, int width, int height) : Window(title, width, height), OgreBites::ApplicationContext(title)
{
    running = true;
    // TODO set window size
    initApp();
    addInputListener(&input);
}

void WindowOgre::processInput() {
    if (input.keys[OgreBites::SDLK_ESCAPE])
        stop();
}

void WindowOgre::update(clock_type delta) {
    this->delta = delta;
    processInput();
    render();
    root->renderOneFrame(delta);
    postProcess();
}

void WindowOgre::render() {
}

void WindowOgre::postProcess() {
}

void WindowOgre::stop() {
    closed = true;
}

bool WindowOgre::isRunning() {
    return !closed && !root->endRenderingQueued();
}

WindowOgre::~WindowOgre() {
    closeApp();
}

void WindowOgre::windowResized(Ogre::RenderWindow *rw) {
    unsigned int width, height, depth;
    rw->getMetrics(width, height, depth);
    windowResized((int)width, (int)height);
}

void WindowOgre::windowResized(int width, int height) {
    Window::windowResized(width, height);
}

void WindowOgre::setup() {
    OgreBites::ApplicationContext::setup();
    root = getRoot();
    scnMgr = root->createSceneManager();
    Ogre::RTShader::ShaderGenerator *shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    Ogre::Light *light = scnMgr->createLight("MainLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    Ogre::SceneNode *lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 0, 100);
    lightNode->attachObject(light);

    camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 0, 100);
    camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);

    cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    cam->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
    cam->setOrthoWindowHeight(50);
    camNode->attachObject(cam);

    // and tell it to render into the main window
    getRenderWindow()->addViewport(cam);
}
