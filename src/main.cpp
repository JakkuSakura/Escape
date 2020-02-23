#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "engine/scene.h"
#include "engine/window_glfw.h"
#include "engine/separate_application.h"
#include "engine/renderer2d.h"
#include "engine/window_ogre.h"
#include <OgreRenderSystem.h>
#include "engine/sprite2d.h"
#include "engine/MyECS.h"
#include "engine/utils.h"
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
class Display : public WindowGLFW
{
    Scene scene;
    Renderer2D renderer;
    Logic *logic;
    World *world;

public:
    Display() : WindowGLFW("Escape", 800, 600)
    {
        windowResized(800, 600);
    }
    void initialize() override
    {
        WindowGLFW::initialize();
        logic = findSystem<Logic>();
        world = logic->getWorld();
    }
    void windowResized(int width, int height) override
    {
        WindowGLFW::windowResized(width, height);
        scene.mat = glm::ortho<float>(-width / 2, width / 2, -height / 2, height / 2);
    }
    virtual void processInput() override
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

class SelectionBox : public Ogre::ManualObject
{
public:
    SelectionBox(const Ogre::String &name)
        : Ogre::ManualObject(name)
    {
        setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
        setUseIdentityProjection(true);
        setUseIdentityView(true);
        setQueryFlags(0);
    }
    virtual ~SelectionBox() {}

    void setCorners(float left, float top, float right, float bottom)
    {
        left = 2 * left - 1;
        right = 2 * right - 1;
        top = 1 - 2 * top;
        bottom = 1 - 2 * bottom;

        clear();
        begin("", Ogre::RenderOperation::OT_LINE_STRIP);
        position(left, top, -1);
        position(right, top, -1);
        position(right, bottom, -1);
        position(left, bottom, -1);
        position(left, top, -1);
        end();

        setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
    }
    void setCorners(const Ogre::Vector2 &topLeft, const Ogre::Vector2 &bottomRight)
    {

        setCorners(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
    }
};

class DisplayOgre : public WindowOgre
{
    Logic *logic;
    World *world;

    SelectionBox *mSelectionBox = nullptr;
public:
    DisplayOgre() : WindowOgre("Escape", 800, 600) {}
    ~DisplayOgre() 
    {
        if(mSelectionBox)
            delete mSelectionBox;
    }
    void initialize() override
    {
        Window::initialize();
        logic = findSystem<Logic>();
        world = logic->getWorld();

        mSelectionBox = new SelectionBox("SelectionBox");
        scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mSelectionBox);
        mSelectionBox->setCorners(-0.5, -0.5, 0.5, 0.5);
        mSelectionBox->setVisible(true);
    }
    virtual void processInput() override
    {
        // ESC exit
        WindowOgre::processInput();

        if (logic->player == nullptr)
            return;
        if (input.mouse[OgreBites::BUTTON_LEFT])
        {
            double x = input.mouse_x, y = input.mouse_y;
            x -= width / 2;
            y = height / 2 - y;
            // std::cerr << "Cursor: " << x << " " << y << std::endl;
            auto pos = logic->player->get<Position>();
            assert(pos.isValid());
            float angle = atan2(y - pos->y, x - pos->x);
            logic->fire(logic->player, angle);
        }

        glm::vec2 vel(0, 0);
        if (input.keys['w'])
            vel.y += 1;
        if (input.keys['s'])
            vel.y += -1;
        if (input.keys['a'])
            vel.x += -1;
        if (input.keys['d'])
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
        // auto &&pos = ent->get<Position>();
        // assert(pos.isValid());
        // auto &&health = ent->get<Health>();
        // assert(health.isValid());
        // float percent = health->health / health->max_health;
        // renderer.drawRect(Rectangle(pos->x, pos->y, 32, 32, 1 - percent, percent, 0));
    }
    void renderBullet(Entity *ent)
    {
        // auto &&pos = ent->get<Position>();
        // assert(pos.isValid());
        // renderer.drawRect(Rectangle(pos->x, pos->y, 2, 2, 1, 0, 0));
    }
    void windowResized(int width, int height) override
    {
        std::cerr << "Resized "  << width << " " << height << std::endl;
    }
};
} // namespace Escape

int main()
{
    Escape::SeparateApplication app(new Escape::DisplayOgre(), new Escape::Logic());
    app.loop();
    return 0;
}