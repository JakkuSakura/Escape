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
#include "agent.h"

namespace Escape
{

class Logic : public SystemManager
{
    class PostInit : public ECSSystem
    {
        Logic *logic;

    public:
        PostInit(Logic *logic) : logic(logic) {}
        void initialize() override
        {
            logic->player = logic->agent_system->createAgent(Position(0, 0));
            logic->agent_system->createAgent(Position(-200, 200));
            logic->agent_system->createAgent(Position(0, 200));
            logic->agent_system->createAgent(Position(200, 200));
        }
    };

public:
    Entity *player;
    MovementSystem *movement_system;
    BulletSystem *bullet_system;
    WeaponSystem *weapon_system;
    LifespanSystem *lifespan_system;
    AgentSystem *agent_system;
    TimeServer *timeserver;
    void initialize() override
    {
        SystemManager::initialize();
        timeserver = findSystem<Application>()->timeserver;
    }
    Logic()
    {
        addSubSystem(movement_system = new MovementSystem());
        addSubSystem(lifespan_system = new LifespanSystem());
        addSubSystem(bullet_system = new BulletSystem());
        addSubSystem(weapon_system = new WeaponSystem());
        addSubSystem(agent_system = new AgentSystem());
        addSubSystem(new PostInit(this));
    }


    void fire(Entity *ent, float angle)
    {
        weapon_system->fire(ent, angle);
    }
    void move(Entity *ent, const glm::vec2 &vel)
    {
        movement_system->move(ent, vel);
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
        Window::initialize();
        logic = findSystem<Logic>();
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

        if (logic->player == nullptr)
            return;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            x -= width / 2;
            y = height / 2 - y;
            // std::cerr << "Cursor: " << x << " " << y << std::endl;
            auto pos = logic->player->get<Position>();
            assert(pos.isValid());
            float angle = atan2(y - pos->y, x - pos->x);
            logic->fire(logic->player, angle);
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
        logic->move(logic->player, vel);
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
        auto &&health = ent->get<Health>();
        assert(health.isValid());
        float percent = health->health / health->max_health;
        renderer.drawRect(Rectangle(pos->x, pos->y, 32, 32, 1 - percent, percent, 0));
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