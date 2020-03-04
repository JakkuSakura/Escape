
#include "ai_system.h"
#include "lua_ai.h"
#include "logic.h"
#include "display.h"
#include "map_converter.h"

using namespace Escape;

int main(int argc, const char **argv) {
    if (argc < 2) {
        std::cerr << "You must specify a map folder" << std::endl;
        exit(-1);
    }
    auto display = new DisplayOgre();
    MapConverter mapConverter;
    auto *world = mapConverter.convert(argv[1]);
    System system;
    auto *logic = new Logic();
    logic->setWorld(world);
    system.addSubSystem(logic);
    system.addSubSystem(display);
    auto ai_system = logic->findSystem<AISystem>();

    auto ai_path = std::string(argv[1]) + "/simple_ai.lua";
    for (entt::entity ent = ai_system->allocate(); ent != entt::null; ent = ai_system->allocate()) {
        ai_system->insert(ent, new Agent_Lua(std::string(ai_path)));
    }

    system.foreach([](System *sys) {
        sys->initialize();
    });
    while (display->isRunning()) {
        system.updateAll(1 / 60.0f);
    }
    return 0;
}