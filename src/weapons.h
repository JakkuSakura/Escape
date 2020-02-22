#if !defined(WEAPONS_H)
#define WEAPONS_H
#include "MyECS.h"
#include "movement.h"
#include "lifespan.h"
namespace Escape
{

enum class BulletType
{
    HANDGUN_BULLET,
    SHOTGUN_SHELL,
    MINIGUN_BULLET,
    RIFLE_BULLET
};
struct BulletData
{
    Entity *firer;
    BulletType type;
};

class BulletSystem : public ECSSystem
{
public:
    void fire(Entity *ent, BulletType type, float angle)
    {
        Entity *bullet = world->create();
        bullet->assign<Name>("bullet");
        bullet->assign<BulletData>(BulletData({ent, type}));
        float speed = 300;
        bullet->assign<Velocity>(speed * cos(angle), speed * sin(angle));
        bullet->assign<Position>(ent->get<Position>().get());
        bullet->assign<Lifespan>(Lifespan({game_clock(), game_clock() + 3}));
    }
};
} // namespace Escape

#endif // WEAPONS_H
