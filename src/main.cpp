#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "engine/separate_application.h"
#include "engine/window_ogre.h"
#include <OgreRenderSystem.h>
#include "engine/MyECS.h"
#include "engine/utils.h"
#include "weapons.h"
#include "movement.h"
#include "serialization.h"
#include "agent.h"
#include <sstream>

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
        timeserver->setRate(60);
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
    void changeWeapon(Entity *ent, WeaponType type)
    {
        // FIXME By changing weapon quickly, the player has a change of shooting each frame
        auto w = ent->get<Weapon>();
        if (w.isValid())
        {
            if (type != w->weapon)
                ent->assign<Weapon>(Weapon{type, 0, 0});
        }
        else
        {
            ent->assign<Weapon>(Weapon{type, 0});
        }
    }
};

using namespace Escape::Render;

class DisplayOgre : public WindowOgre
{
    Logic *logic;
    World *world;
    Ogre::SceneNode *rects;

public:
    DisplayOgre() : WindowOgre("Escape", 800, 600) {}
    ~DisplayOgre()
    {
    }

    std::pair<Ogre::SceneNode *, Ogre::Entity *> newBox(float cx, float cy, float width, float height, float r, float g, float b)
    {
        auto pair = newBox(cx, cy, width, height);
        Ogre::Entity *cube = pair.second;
        char name[32];
        sprintf(name, "Color(%d,%d,%d)", (int)(r * 255), (int)(g * 255), (int)(b * 255));
        auto material = Ogre::MaterialManager::getSingleton().getByName(name);
        if (!material)
        {
            material = Ogre::MaterialManager::getSingleton().getByName("white")->clone(name);
            material->setDiffuse(r, g, b, 1);
        }
        cube->setMaterial(material);
        return pair;
    }

    std::pair<Ogre::SceneNode *, Ogre::Entity *> newBox(float cx, float cy, float width, float height)
    {
        Ogre::SceneNode *cubeNode = rects->createChildSceneNode();
        cubeNode->setScale(width / 100, height / 100, 1.0 / 100);
        cubeNode->setPosition(cx, cy, 0);

        Ogre::Entity *cube = scnMgr->createEntity("Prefab_Cube");
        cube->setMaterialName("white");

        cubeNode->attachObject(cube);
        return std::make_pair(cubeNode, cube);
    }
    Ogre::Vector3 pickUp(unsigned int absoluteX, unsigned int absoluteY)
    {
        float width = (float)cam->getViewport()->getActualWidth();   // viewport width
        float height = (float)cam->getViewport()->getActualHeight(); // viewport height

        Ogre::Ray ray = cam->getCameraToViewportRay((float)absoluteX / width, (float)absoluteY / height);
        float t = ray.getOrigin().z / ray.getDirection().z;

        return ray.getOrigin() + ray.getDirection() * t;
    }
    void initialize() override
    {
        Window::initialize();
        logic = findSystem<Logic>();
        world = logic->getWorld();
        rects = scnMgr->getRootSceneNode()->createChildSceneNode();
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Popular");
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets", "FileSystem", "Popular");
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Popular");
        Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("Popular");
    }
    virtual void processInput() override
    {
        // ESC exit
        WindowOgre::processInput();

        if (logic->player == nullptr)
            return;
        if (input.mouse[OgreBites::BUTTON_LEFT])
        {
            auto click = pickUp(input.mouse_x, input.mouse_y);
            double x = click.x, y = click.y;
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
            vel *= 120.0f;
        }
        logic->move(logic->player, vel);

        if (input.keys['1'])
            logic->changeWeapon(logic->player, WeaponType::HANDGUN);

        if (input.keys['2'])
            logic->changeWeapon(logic->player, WeaponType::SHOTGUN);

        if (input.keys['3'])
            logic->changeWeapon(logic->player, WeaponType::SMG);

        if (input.keys['4'])
            logic->changeWeapon(logic->player, WeaponType::RIFLE);

        if (input.keys['p'])
            logic->agent_system->createAgent(Position(logic->timeserver->random(-200, 200), logic->timeserver->random(-200, 200)));

        if (input.keys['o'])
        {
            try
            {
                std::cerr << "Writing map file" << std::endl;
                SerializationHelper helper("map.txt");
                helper.serialize_ptr(world);
            }
            catch (std::runtime_error &e)
            {
                std::cerr << "error " << e.what() << std::endl;
            }
            catch (std::exception &e)
            {
                std::cerr << "error " << e.what() << std::endl;
            }
        }
        if (input.keys['i'])
        {
            try
            {
                std::cerr << "Reading map file" << std::endl;
                SerializationHelper helper("map.txt");
                helper.deserialize_ptr(world);
                logic->player = world->getById(1);
                // FIXME sometimes with bullets flying it chrushes
            }
            catch (std::runtime_error &e)
            {
                std::cerr << "error " << e.what() << std::endl;
            }
            catch (std::exception &e)
            {
                std::cerr << "error " << e.what() << std::endl;
            }
        }
    }
    void render() override
    {
        // std::cerr << "Render " << logic->timeserver->getTick() << std::endl;
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
    virtual void postProcess() override
    {
        rects->removeAndDestroyAllChildren();
    }
    void renderAgent(Entity *ent)
    {
        auto &&pos = ent->get<Position>();
        assert(pos.isValid());
        auto &&health = ent->get<Health>();
        assert(health.isValid());
        float percent = health->health / health->max_health;
        newBox(pos->x, pos->y, 32, 32, 1 - percent, percent, 0);
    }
    void renderBullet(Entity *ent)
    {
        auto &&pos = ent->get<Position>();
        assert(pos.isValid());
        newBox(pos->x, pos->y, 2, 2);
    }
    void windowResized(int width, int height) override
    {
        std::cerr << "Resized " << width << " " << height << std::endl;
    }
};
} // namespace Escape

int main()
{
    Escape::SeparateApplication app(new Escape::DisplayOgre(), new Escape::Logic());
    app.loop();
    return 0;
}