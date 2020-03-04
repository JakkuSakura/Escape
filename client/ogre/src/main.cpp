
#include "ai_system.h"
#include "lua_ai.h"
#include "logic.h"
#include "display.h"
#include "map_converter.h"

using namespace Escape;

class MySystem : public ECSSystem {
    std::string mapfile;
public:
    MySystem(std::string &&map) : mapfile(std::move(map)) {
        MapConverter mapConverter;
        auto *world = mapConverter.convert(mapfile);
        auto *logic = new Logic(world);
        addSubSystem(logic);
        configure();
    }

    void update(float delta) override {
        auto ai_system = findSystem<AISystem>();
        for (entt::entity ent = ai_system->allocate(); ent != entt::null; ent = ai_system->allocate()) {
            auto *world = getWorld();
            auto data = world->get<AgentData>(ent);
            auto ai_path = mapfile + "/" + data.ai + ".lua";
            ai_system->insert(ent, new Agent_Lua(std::move(ai_path)));
        }
    }
};

int main(int argc, const char **argv) {
    if (argc < 2) {
        std::cerr << "You must specify a map folder" << std::endl;
        exit(-1);
    }

    auto display = new DisplayOgre();

    MySystem system(argv[1]);
    system.addSubSystem(display);

    system.foreach([](System *sys) {
        sys->initialize();
    });
    while (display->isRunning()) {
        system.updateAll(1 / 60.0f);
    }
    return 0;
}