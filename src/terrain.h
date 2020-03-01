#if !defined(TERRAIN_H)
#define TERRAIN_H
#include "MyECS.h"
#include "components.h"
namespace Escape
{
class TerrainSystem : public ECSSystem {
public:
    static entt::entity createWall(World *world, float x, float y, float w, float h) {
        entt::entity wall = world->create();
        world->assign<Position>(wall, x, y);
        world->assign<Name>(wall, "box");
		world->assign<Rotation>(wall, 0);
        world->assign<TerrainData>(wall, TerrainType::BOX, w, h, 0.0f, 0.0f);
		return wall;
    }
};
    
} // namespace Escape


#endif // TERRAIN_H
