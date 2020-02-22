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
#include "MyECS.h"
using namespace Escape;
using namespace ECS;

struct Position : public glm::vec2
{
    using glm::vec2::vec2;
};
struct Velocity : public glm::vec2
{
    using glm::vec2::vec2;
};

struct Name : public std::string
{
    using std::string::string;
};

class MovementSystem : public ECSSystem
{
public:
    void update(float delta) override
    {
        world->each<Velocity>([&](Entity *ent, ComponentHandle<Velocity> vel) {
            auto &&pos = ent->get<Position>();
            assert(pos.isValid());
            pos.get() += vel.get() * delta;
        });
    }

private:
};
struct Bullet
{
    Entity *firer;

};
class BulletSystem : public ECSSystem
{
public:
};
class Logic : public SystemManager
{
public:
    Entity *player;
    Logic()
    {
        addSubSystem(movementSystem = new MovementSystem());

        player = createAgent(Position(0, 0));
    }
    Entity *createAgent(const Position &pos)
    {
        Entity *agent = getWorld()->create();
        agent->assign<Name>("Agent");
        agent->assign<Position>(pos);
        return agent;
    }
    MovementSystem *movementSystem;
};

using namespace Escape::Render;
class Display : public Window
{
    Scene scene;
    Renderer2D renderer;
    Logic *logic;
    World *world;

public:
    Display() : Window("Display", 800, 600)
    {
        windowResized(800, 600);
    }
    void initialize() override
    {
        logic = findSystem<Logic>();
        assert(logic != nullptr);
        world = logic->getWorld();
    }
    void windowResized(int width, int height) override
    {
        Window::windowResized(width, height);
        scene.mat = glm::ortho<float>(-width / 2, width / 2, -height / 2, height / 2);
    }
    virtual void processInput(GLFWwindow *window) override
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            x -= width / 2;
            y = height / 2 - y;
            std::cerr << "Cursor: " << x << " " << y << std::endl;
        }

        glm::vec2 vel(0, 0);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            vel.y += 1;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            vel.y += -1;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            vel.x += -1;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            vel.x += 1;
        float spd = glm::length(*(glm::vec2 *)&vel);
        if (spd > 0)
        {
            if (spd > 1)
            {
                vel /= spd;
            }
            vel *= 30.0f;
            logic->player->assign<Velocity>(vel.x, vel.y);
        }
        else
        {
            logic->player->remove<Velocity>();
        }
    }
    void render() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.applyScene(scene);
        world->each<Name>([&, this](Entity *ent, ComponentHandle<Name> name) {
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