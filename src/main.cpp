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
#include "utils.h"
#include "weapons.h"
#include "movement.h"
using namespace ECS;
namespace Escape
{


class Logic : public SystemManager
{
public:
    Entity *player;
    MovementSystem *movementSystem;
    BulletSystem *bulletSystem;
    LifespanSystem *lifespanSystem;
    Logic()
    {
        addSubSystem(movementSystem = new MovementSystem());
        addSubSystem(bulletSystem = new BulletSystem());
        addSubSystem(lifespanSystem = new LifespanSystem());

        player = createAgent(Position(0, 0));
    }
    Entity *createAgent(const Position &pos)
    {
        Entity *agent = getWorld()->create();
        agent->assign<Name>("agent");
        agent->assign<Position>(pos);
        return agent;
    }
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

            float angle = atan2(y - logic->player->get<Position>()->y, x - logic->player->get<Position>()->x);
            logic->bulletSystem->fire(logic->player, BulletType::MINIGUN_BULLET, angle);
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
            vel *= 64.0f;
        }
        logic->movementSystem->move(logic->player, vel);
    }
    void render() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.applyScene(scene);
        world->each<Name>([&, this](Entity *ent, ComponentHandle<Name> name) {
            if (name.get() == "agent")
            {
                renderAgent(ent);
            }
            if (name.get() == "bullet")
            {
                renderBullet(ent);
            }
        });
    }
    void renderAgent(Entity *ent)
    {
        auto &&pos = ent->get<Position>();
        assert(pos.isValid());
        renderer.drawRect(Rectangle(pos->x, pos->y, 32, 32, 1, 1, 1));
    }
    void renderBullet(Entity *ent)
    {
        auto &&pos = ent->get<Position>();
        assert(pos.isValid());
        renderer.drawRect(Rectangle(pos->x, pos->y, 2, 2, 1, 0, 0));
    }
};
} // namespace Escape

int main()
{
    Escape::SeparateApplication app(new Escape::Display(), new Escape::Logic());
    app.loop();
    return 0;
}