#if !defined(TERRAIN_H)
#define TERRAIN_H

#include "MyECS.h"
#include "components.h"

namespace Escape {
    class TerrainSystem : public ECSSystem {
    public:
        TerrainSystem() {

        }
        static entt::entity createWall(World *world, float x, float y, float w, float h) {
            entt::entity wall = world->create();
            world->assign<Position>(wall, x, y);
            world->assign<Name>(wall, "wall");
            world->assign<Rotation>(wall, 0);
            world->assign<TerrainData>(wall, TerrainType::WALL, TerrainShape::BOX, w, h, 0.0f, 0.0f);
            return wall;
        }

        static entt::entity createWater(World *world, float x, float y, float w, float h) {
            entt::entity water = world->create();
            world->assign<Position>(water, x, y);
            world->assign<Name>(water, "water");
            world->assign<Rotation>(water, 0);
            world->assign<TerrainData>(water, TerrainType::WATER, TerrainShape::BOX, w, h, 0.0f, 0.0f);
            return water;
        }
        static entt::entity createBarrier(World *world, float x, float y, float w, float h) {
            entt::entity barrier = world->create();
            world->assign<Position>(barrier, x, y);
            world->assign<Name>(barrier, "barrier");
            world->assign<Rotation>(barrier, 0);
            world->assign<TerrainData>(barrier, TerrainType::BARRAIR, TerrainShape::BOX, w * 0.5f, h * 0.5f, 0.0f, 0.0f);
            // TODO some special process to make barriers interconnected both physically and visually
            return barrier;
        }
    };

} // namespace Escape


#endif // TERRAIN_H
