#if !defined(WEAPONS_H)
#define WEAPONS_H
#include "MyECS.h"
#include "movement.h"
#include "engine/utils.h"
#include "components.h"
#include "lifespan.h"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <map>
#include <cmath>
#include <random>

namespace Escape
{
class BulletSystem : public ECSSystem
{
    LifespanSystem *lifespan;

public:
    BulletSystem()
    {
    }
    void initialize() override
    {
        ECSSystem::initialize();
        lifespan = findSystem<LifespanSystem>();
    }
    void fire(Entity firer, BulletType type, float angle, float speed, float damage)
    {
        Entity bullet = world->create();
        world->assign<Name>(bullet, "bullet");
        world->assign<BulletData>(bullet, BulletData{firer_id : entt::to_integer(firer), type : type, damage : damage, hit : false});
        world->assign<Velocity>(bullet, speed * cos(angle), speed * sin(angle));
        world->assign<Position>(bullet, world->get<Position>(firer));
        world->assign<Lifespan>(bullet, lifespan->period(3));
    }
    void update(clock_type delta) override
    {
        world->view<Hitbox, Position, Health>().each([&](Entity ent, auto &hitbox, auto &position, auto &health) {
            if (health.health <= 0)
                return;

            world->view<BulletData, Position>().each([&](Entity bullet, auto &bullet_data, auto &position_bullet) {
                if (bullet_data.firer_id == entt::to_integer(ent))
                    return;
                if (glm::distance2(position_bullet.unwrap(), position.unwrap()) <= hitbox.radius * hitbox.radius) // hit
                {
                    health.health -= bullet_data.damage;
                    bullet_data.hit = true;
                    world->destroy(bullet);
                    return;
                }
            });
        });
    }
};
class WeaponSystem : public ECSSystem
{
    BulletSystem *bullet_system;
    TimeServer *timeserver;
    std::map<WeaponType, WeaponPrototype> default_weapons;

public:
    WeaponSystem()
    {
        default_weapons[WeaponType::HANDGUN] = WeaponPrototype{
            type : WeaponType::HANDGUN,
            bullet_type : BulletType::HANDGUN_BULLET,
            cd : 0.5,
            accuracy : 95,
            peice_number : 1,
            bullet_damage : 10,
            bullet_speed : 300,
        };
        default_weapons[WeaponType::SHOTGUN] = WeaponPrototype{
            type : WeaponType::SHOTGUN,
            bullet_type : BulletType::SHOTGUN_SHELL,
            cd : 1.5,
            accuracy : 80,
            peice_number : 10,
            bullet_damage : 8,
            bullet_speed : 300,
        };
        default_weapons[WeaponType::SMG] = WeaponPrototype{
            type : WeaponType::SMG,
            bullet_type : BulletType::SMG_BULLET,
            cd : 1.0 / 40,
            accuracy : 85,
            peice_number : 1,
            bullet_damage : 3,
            bullet_speed : 300,
        };
        default_weapons[WeaponType::RIFLE] = WeaponPrototype{
            type : WeaponType::RIFLE,
            bullet_type : BulletType::RIFLE_BULLET,
            cd : 2,
            accuracy : 97,
            peice_number : 1,
            bullet_damage : 55,
            bullet_speed : 500,
        };
    }
    void initialize() override
    {
        ECSSystem::initialize();
        bullet_system = findSystem<BulletSystem>();
        timeserver = findSystem<TimeServer>();
    }
    void fire(Entity ent, float angle)
    {
        TRACE();
        if (world->has<Weapon>(ent))
        {
            TRACE();
            auto weapon = world->get<Weapon>(ent);
            TRACE();

            const WeaponPrototype &prototype = default_weapons.at(weapon.weapon);
            if (timeserver->now() >= weapon.next)
            {
                float angle_diff = std::max(0.0, M_PI_4 * (100 - prototype.accuracy) / 100);
                TRACE();

                for (size_t i = 0; i < prototype.peice_number; i++)
                {
                    TRACE();

                    bullet_system->fire(ent, prototype.bullet_type, angle + timeserver->random(-angle_diff, angle_diff), prototype.bullet_speed, prototype.bullet_damage);

                    TRACE();
                }
                weapon.last = timeserver->now();
                TRACE();

                weapon.next = timeserver->now() + prototype.cd;
            }
        }
    }
};
} // namespace Escape

#endif // WEAPONS_H
