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
using namespace Escape;
class Demo : public Window
{
    Scene scene;
    Renderer2D renderer;
public:
    Demo() : Window("Demo", 800, 600)
    {
    }
    void render() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.applyScene(scene);
        renderer.drawRect();
    }
};

int main()
{
    Application app(new Demo(), new Core());
    app.loop();
    return 0;
}