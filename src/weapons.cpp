#include "weapons.h"
#include "components.h"

namespace Escape {

    BulletSystem::BulletSystem() {
    }

    void BulletSystem::initialize() {
        ECSSystem::initialize();
        lifespan = findSystem<LifespanSystem>();
    }

    void BulletSystem::fire(Entity firer, BulletType type, float angle, float speed, float damage, float distance) {
        Entity bullet = world->create();
        world->assign<Name>(bullet, "bullet");
        auto data = BulletData{.firer_id =  world->get<AgentData>(firer).id,
                .type =  type,
                .damage =  damage,
                .density = 7.6,
                .radius = 0.2,
                .hit =  false};
        world->assign<BulletData>(bullet, data);
        world->assign<Hitbox>(bullet, Hitbox{.radius =  data.radius});

        glm::vec2 ang(cos(angle), sin(angle));

        world->assign<Velocity>(bullet, speed * ang);
        world->assign<Position>(bullet, world->get<Position>(firer) + ang * distance);
        world->assign<Lifespan>(bullet, lifespan->period(3));
    }

    void BulletSystem::update(clock_type delta) {
        world->view<Hitbox, Position, Health, AgentData>().each(
                [&](Entity ent, auto &hitbox, auto &position, auto &health, auto id) {
                    if (health.health <= 0)
                        return;

                    world->view<BulletData, Position>().each(
                            [&](Entity bullet, auto &bullet_data, auto &position_bullet) {
                                if (bullet_data.firer_id == id.id)
                                    return;
                                if (glm::distance2(position_bullet.unwrap(), position.unwrap()) <=
                                    hitbox.radius * hitbox.radius) // hit
                                {
                                    health.health -= bullet_data.damage;
                                    bullet_data.hit = true;
                                    world->destroy(bullet);
                                    return;
                                }
                            });
                });
    }

    WeaponSystem::WeaponSystem() {
        default_weapons[WeaponType::HANDGUN] = WeaponPrototype{
                .type =  WeaponType::HANDGUN,
                .bullet_type =  BulletType::HANDGUN_BULLET,
                .cd =  0.5,
                .accuracy =  95,
                .bullet_number =  1,
                .bullet_damage =  10,
                .bullet_speed =  60,
                .gun_length =  2.1,
        };
        default_weapons[WeaponType::SHOTGUN] = WeaponPrototype{
                .type =  WeaponType::SHOTGUN,
                .bullet_type =  BulletType::SHOTGUN_SHELL,
                .cd =  1.5,
                .accuracy = 80,
                .bullet_number = 10,
                .bullet_damage = 8,
                .bullet_speed = 60,
                .gun_length = 2.5,
        };
        default_weapons[WeaponType::SMG] = WeaponPrototype{
                .type = WeaponType::SMG,
                .bullet_type = BulletType::SMG_BULLET,
                .cd = 1.0 / 30,
                .accuracy = 85,
                .bullet_number = 1,
                .bullet_damage = 4,
                .bullet_speed = 60,
                .gun_length = 2.3,
        };
        default_weapons[WeaponType::RIFLE] = WeaponPrototype{
                .type = WeaponType::RIFLE,
                .bullet_type = BulletType::RIFLE_BULLET,
                .cd = 2,
                .accuracy = 97,
                .bullet_number = 1,
                .bullet_damage = 55,
                .bullet_speed = 100,
                .gun_length = 3,
        };
    }

    void WeaponSystem::initialize() {
        ECSSystem::initialize();
        bullet_system = findSystem<BulletSystem>();
        timeserver = findSystem<TimeServer>();
    }

    void WeaponSystem::fire(Entity ent, float angle) {
        if (world->has<Weapon>(ent)) {
            auto &weapon = world->get<Weapon>(ent);
            if (timeserver->now() >= weapon.next) {
                const WeaponPrototype &prototype = default_weapons.at(weapon.weapon);
                float angle_diff = std::max(0.0, M_PI_4 * (100 - prototype.accuracy) / 100);

                for (size_t i = 0; i < prototype.bullet_number; i++) {

                    bullet_system->fire(ent, prototype.bullet_type, angle + timeserver->random(-angle_diff, angle_diff),
                                        prototype.bullet_speed, prototype.bullet_damage, prototype.gun_length);
                }
                weapon.last = timeserver->now();
                weapon.next = timeserver->now() + prototype.cd;
            }
        }
    }
    void WeaponSystem::changeWeapon(Entity ent, WeaponType type) {
        // FIXME By changing weapon quickly, the player has a change of shooting each frame
        if (world->has<Weapon>(ent)) {
            auto &w = world->get<Weapon>(ent);
            if (type != w.weapon)
                world->assign_or_replace<Weapon>(ent, Weapon{type, 0, 0});
        } else {
            world->assign<Weapon>(ent, Weapon{type, 0});
        }
    }

    void WeaponSystem::update(clock_type delta) {
        world->view<ChaneWeapon>().each([&](auto ent, auto &chg) {
            if (!chg.processed) {
                changeWeapon(ent, chg.weapon);
                chg.processed = true;
            }
        });

        world->view<Shooting>().each([&](auto ent, auto &sht) {
            if (!sht.processed) {
                fire(ent, sht.angle);
                sht.processed = true;
            }
        });
    }
} // namespace Escape