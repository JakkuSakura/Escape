#include "read_map.h"
#include <fstream>
#include <cstdio>
#include <sstream>
#include <json.hpp>
#include "agent.h"
using json = nlohmann::json;
namespace Escape
{

World *ReadMap::readMapFile(World *world)
{

    std::cerr << "Reading file " << filename << " as map" << std::endl;
    std::ifstream is(filename);
    json js;
    is >> js;
    std::cerr << js << std::endl;
    for (int i = 0, size = js["AGENTS"].size(); i < size; ++i)
    {
        auto &agt = js["AGENTS"][i];
        Position pos(agt["X"].get<float>(), agt["Y"].get<float>());
        std::string type = agt["TYPE"].get<std::string>();
        std::string guntype = agt["WEAPON"].get<std::string>();

        Weapon wp{WeaponType::HANDGUN, 0, 0};

        if (guntype == "HANDGUN")
        {
            wp.weapon = WeaponType::HANDGUN;
        }
        else if (guntype == "SHOTGUN")
        {
            wp.weapon = WeaponType::SHOTGUN;
        }
        else if (guntype == "SMG")
        {
            wp.weapon = WeaponType::SMG;
        }
        else if (guntype == "RIFLE")
        {
            wp.weapon = WeaponType::RIFLE;
        }
        else
        {
            std::cerr << "Undefined weapon type: " << guntype << std::endl;
        }
        int player = 0;
        if (type == "PLAYER")
            player = 1;
        
        auto entity = AgentSystem::createAgent(world, pos, player);
        world->assign_or_replace<Weapon>(entity, wp);
    }

    for (int i = 0, size = js["TERRAINS"].size(); i < size; ++i)
    {
        auto &ter = js["TERRAINS"][i];
        if (ter["TYPE"] == "BOX")
        {

            Position pos(ter["X"].get<float>(), ter["Y"].get<float>());
            Rotation rot{ter["ROTATION"].get<float>() / (float)M_PI * 180};
            TerrainData data{type : TerrainType::BOX, {ter["WIDTH"].get<float>(), ter["HEIGHT"].get<float>()}};

            auto entity = world->create();
            world->assign<Name>(entity, "box");
            world->assign<Position>(entity, pos);
            world->assign<Rotation>(entity, rot);
            world->assign<TerrainData>(entity, data);
        }
    }
    return world;
}

ReadMap::ReadMap(const std::string &filename) : filename(filename)
{
}

ReadMap::~ReadMap()
{
}
} // namespace Escape