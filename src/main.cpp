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

class MovementSystem : public System
{
    std::vector<std::pair<Entity *, glm::vec2>> movement_events;

public:
    void move(Entity *ent, const glm::vec2 &speed)
    {
        movement_events.emplace_back(ent, speed);
    }
    void update(float delta) override
    {
        for (auto &&pr : movement_events)
        {
            assert(pr.first->get<Position>().isValid());
            pr.first->get<Position>().get() += pr.second * delta;
        }

        movement_events.clear();
    }

private:
};
class Logic : public ECSCore
{
public:
    Logic()
    {
        addSubSystem(movementSystem = new MovementSystem());

        createAgent(Position(66, 33));
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

class Display : public Window
{
    Scene scene;
    Renderer2D renderer;
    World *world;

public:
    Display() : Window("Display", 800, 600)
    {
        windowResized(800, 600);
    }
    void initialize() override
    {
        auto system = findSystem<ECSCore>();
        assert(system != nullptr);
        world = system->getWorld();
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