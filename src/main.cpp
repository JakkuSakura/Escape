#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <glm/mat4x4.hpp>
#include "scene.h"
#include "window.h"
#include "application.h"
#include "renderer2d.h"
#include "core.h"
#include "sprite2d.h"
using namespace Escape;
class Demo : public Window
{
    Scene scene;
    Renderer2D renderer;
    Rectangle rect;

public:
    Demo() : Window("Demo", 800, 600), rect(0, 0, 100, 100, 1, 0, 0)
    {
        windowResized(800, 600);
    }
    void windowResized(int width, int height) override
    {
        Window::windowResized(width, height);
        scene.mat = glm::ortho<float>(-width / 2, width / 2, -height / 2, height / 2);
    }
    void render() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.applyScene(scene);
        renderer.drawRect(rect);
    }
};

int main()
{
    Application app(new Demo(), new Core());
    app.loop();
    return 0;
}