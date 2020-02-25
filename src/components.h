#if !defined(COMPONENTS_H)
#define COMPONENTS_H
#include "engine/utils.h"
#include "MyECS.h"
#include <glm/glm.hpp>
namespace Escape
{
COMPONENT_AS(Name, std::string);
COMPONENT_AS(Position, glm::vec2);
COMPONENT_AS(Velocity, glm::vec2);

COMPONENT_NEW(ID, 
    size_t id;
);

COMPONENT_NEW(Hitbox,
    float radius;
);

COMPONENT_NEW(Rotation,
    float radian;
);

COMPONENT_NEW(Control,
    int player;
);

COMPONENT_NEW(TimeServerInfo,
    size_t tick;
);

COMPONENT_NEW(Lifespan,
    clock_type begin;
    clock_type end;
);

COMPONENT_NEW(Health,
    float health;
    float max_health;
    Health(const Health &h) = default;
    Health(float h = 100)
    {
        health = h;
        max_health = h;
    }
);

enum class BulletType
{
    HANDGUN_BULLET,
    SHOTGUN_SHELL,
    SMG_BULLET,
    RIFLE_BULLET
};
COMPONENT_NEW(BulletData,
    size_t firer_id;
    BulletType type;
    float damage;
    bool hit;
);

enum class WeaponType
{
    HANDGUN,
    SHOTGUN,
    SMG,
    RIFLE
};
COMPONENT_NEW(WeaponPrototype,
    WeaponType type;
    BulletType bullet_type;
    float cd;
    float accuracy;
    float peice_number;
    float bullet_damage;
    float bullet_speed;
);

COMPONENT_NEW(Weapon,
    WeaponType weapon;
    float last;
    float next;
);
enum class Terrain {
    BOX,
    CIRCLE
};
COMPONENT_NEW(TerrainData,
    Terrain type;
    float arguments[16];
);

} // namespace Escape
#define FOREACH_COMPONENT_TYPE(func) \
    func(Escape::Position); \
    func(Escape::Name); \
    func(Escape::Rotation); \
    func(Escape::ID); \
    func(Escape::Velocity); \
    func(Escape::Health); \
    func(Escape::Weapon); \
    func(Escape::WeaponPrototype); \
    func(Escape::Hitbox); \
    func(Escape::BulletData); \
    func(Escape::Lifespan); \
    func(Escape::TimeServerInfo); \
    func(Escape::Control); \
    func(Escape::TerrainData);

    
#endif // COMPONENTS_H
