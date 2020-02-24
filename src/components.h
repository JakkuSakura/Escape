#if !defined(COMPONENTS_H)
#define COMPONENTS_H
#include "engine/utils.h"
namespace Escape
{
NEW_TYPE(Name, std::string);
NEW_TYPE(Position, glm::vec2);
NEW_TYPE(Velocity, glm::vec2);

struct Hitbox
{
    float radius;
};

struct Lifespan
{
    clock_type begin;
    clock_type end;
};

struct Health
{
    float health;
    float max_health;
    Health(const Health &h) = default;
    Health(float h)
    {
        health = h;
        max_health = h;
    }
};

enum class BulletType
{
    HANDGUN_BULLET,
    SHOTGUN_SHELL,
    SMG_BULLET,
    RIFLE_BULLET
};
struct BulletData
{
    size_t firer_id;
    BulletType type;
    float damage;
    bool hit;
};

enum class WeaponType
{
    HANDGUN,
    SHOTGUN,
    SMG,
    RIFLE
};
struct WeaponPrototype
{
    WeaponType type;
    BulletType bullet_type;
    float cd;
    float accuracy;
    float peice_number;
    float bullet_damage;
    float bullet_speed;
};

struct Weapon
{
    WeaponType weapon;
    float last;
    float next;
};

} // namespace Escape

#endif // COMPONENTS_H
