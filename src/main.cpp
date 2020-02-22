#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <glm/mat4x4.hpp>
#include "scene.h"
#include "window.h"
#include "separate_application.h"
#include "renderer2d.h"
#include "system.h"
#include "sprite2d.h"
#include "ECScore.h"
using namespace Escape;
using namespace ECS;

struct Position : public glm::vec2
{
    using glm::vec2::vec2;
};
struct Name : public std::string
{
    using std::string::string;
};
class Logic : public ECSCore
{
public:
    Logic()
    {
        createAgent(Position(66, 33));
    }
    Entity *createAgent(const Position &pos)
    {
        Entity *agent = getWorld()->create();
        agent->assign<Name>("Agent");
        agent->assign<Position>(pos);
        return agent;
    }
};

class Display : public Window
{
    Scene scene;
    Renderer2D renderer;

public:
    Display() : Window("Display", 800, 600)
    {
        windowResized(800, 600);
    }
    void windowResized(int width, int height) override
    {
        Window::windowResized(width, height);
        scene.mat = glm::ortho<float>(-width / 2, width / 2, -height / 2, height / 2);
    }
    World *getWorld() const
    {
        assert(parent != nullptr);
        return ((Logic *)((SeparateApplication *)parent)->getCore())->getWorld();
    }
    void render() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.applyScene(scene);
        getWorld()->each<Name>([&, this](Entity *ent, ComponentHandle<Name> name) {
            if (name.get() == "Agent")
            {
                renderAgent(ent);
            }
        });
    }
    void renderAgent(Entity *ent)
    {
        auto &&pos = ent->get<Position>();
        renderer.drawRect(Rectangle(pos->x, pos->y, 32, 32, 1, 1, 1));
    }
};
int main()
{
    SeparateApplication app(new Display(), new Logic());
    app.loop();
    return 0;
}