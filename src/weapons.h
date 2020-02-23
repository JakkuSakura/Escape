#if !defined(WEAPONS_H)
#define WEAPONS_H
#include "engine/MyECS.h"
#include "movement.h"
#include "lifespan.h"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <map>
#include <cmath>
#include <random>

namespace Escape
{
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
struct BulletPrototype
{
    BulletType type;
    float damage;
    float speed;
};
struct BulletData
{
    size_t firer_id;
    BulletType type;
    float damage;
    bool hit;
};

class BulletSystem : public ECSSystem
{
    std::map<BulletType, BulletPrototype> default_bullets;
    LifespanSystem *lifespan;

public:
    BulletSystem()
    {
        default_bullets[BulletType::HANDGUN_BULLET] = BulletPrototype{
            type : BulletType::HANDGUN_BULLET,
            damage : 10,
            speed : 300
        };
        default_bullets[BulletType::SHOTGUN_SHELL] = BulletPrototype{
            type : BulletType::SHOTGUN_SHELL,
            damage : 8,
            speed : 300
        };
        default_bullets[BulletType::SMG_BULLET] = BulletPrototype{
            type : BulletType::SMG_BULLET,
            damage : 3,
            speed : 300
        };
        default_bullets[BulletType::RIFLE_BULLET] = BulletPrototype{
            type : BulletType::RIFLE_BULLET,
            damage : 55,
            speed : 500
        };
    }
    void initialize() override
    {
        ECSSystem::initialize();
        lifespan = findSystem<LifespanSystem>();
    }
    void fire(Entity *firer, BulletType type, float angle)
    {
        auto prototype = default_bullets.at(type);
        Entity *bullet = world->create();
        bullet->assign<Name>("bullet");
        bullet->assign<BulletData>(BulletData{firer_id : firer->getEntityId(), type : type, damage : prototype.damage, hit : false});
        bullet->assign<Velocity>(prototype.speed * cos(angle), prototype.speed * sin(angle));
        bullet->assign<Position>(firer->get<Position>().get());
        bullet->assign<Lifespan>(lifespan->period(3));
    }
    void update(clock_type delta) override
    {
        world->each<Hitbox>([&](Entity *ent, ComponentHandle<Hitbox> hitbox) {
            auto health = ent->get<Health>();
            if (health <= 0)
                return;
            auto position = ent->get<Position>();
            if (health.isValid() && position.isValid())
            {
                world->each<BulletData>([&](Entity *bullet, ComponentHandle<BulletData> bullet_data) {
                    if (bullet_data->firer_id == ent->getEntityId())
                        return;
                    auto position_bullet = bullet->get<Position>();
                    // assert(position_bullet.isValid());
                    if (glm::distance2(position_bullet->as_vec2(), position->as_vec2()) <= hitbox->radius * hitbox->radius) // hit
                    {
                        health->health -= bullet_data->damage;
                        bullet_data->hit = true;
                        world->destroy(bullet);
                        return;
                    }
                });
            }
        });
    }
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
    BulletType bullet;
    float cd;
    float accuracy;
    float peice_number;
};
struct Weapon
{
    WeaponType weapon;
    float last;
    float next;
};
class WeaponSystem : public ECSSystem
{
    BulletSystem *bullet_system;
    TimeServer *timeserver;
    std::map<WeaponType, WeaponPrototype> default_weapons;
    std::default_random_engine engine;
public:
    WeaponSystem()
    {
        default_weapons[WeaponType::HANDGUN] = WeaponPrototype{
            type : WeaponType::HANDGUN,
            bullet : BulletType::HANDGUN_BULLET,
            cd : 0.5,
            accuracy : 95,
            peice_number: 1
        };
        default_weapons[WeaponType::SHOTGUN] = WeaponPrototype{
            type : WeaponType::SHOTGUN,
            bullet : BulletType::SHOTGUN_SHELL,
            cd : 1.5,
            accuracy : 80,
            peice_number: 10
        };
        default_weapons[WeaponType::SMG] = WeaponPrototype{
            type : WeaponType::SMG,
            bullet : BulletType::SMG_BULLET,
            cd : 1.0 / 40,
            accuracy : 85,
            peice_number: 1
        };
        default_weapons[WeaponType::RIFLE] = WeaponPrototype{
            type : WeaponType::RIFLE,
            bullet : BulletType::RIFLE_BULLET,
            cd : 2,
            accuracy : 97,
            peice_number: 1
        };
    }
    void initialize() override
    {
        bullet_system = findSystem<BulletSystem>();
        timeserver = findSystem<TimeServer>();
    }
    void fire(Entity *ent, float angle)
    {
        auto weapon = ent->get<Weapon>();
        if (weapon.isValid())
        {
            const WeaponPrototype &prototype = default_weapons.at(weapon->weapon);
            if (timeserver->now() >= weapon->next)
            {
                float angle_diff = std::max(0.0, M_PI_4 * (100 - prototype.accuracy) / 100);
                std::uniform_real_distribution<double> u(-angle_diff, angle_diff);
                for (size_t i = 0; i < prototype.peice_number; i++)
                {
                    bullet_system->fire(ent, prototype.bullet, angle + u(engine));
                }
                weapon->last = timeserver->now();
                weapon->next = timeserver->now() + prototype.cd;
                
            }
        }
    }
};
} // namespace Escape

#endif // WEAPONS_H
