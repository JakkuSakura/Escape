#include "weapons.h"
#include "components.h"
#include "message.h"

namespace Escape {

    BulletSystem::BulletSystem() {
    }

    void BulletSystem::initialize() {
        ECSSystem::initialize();
        lifespan = findSystem<LifespanSystem>();
    }

    void BulletSystem::fire(entt::entity firer, BulletType type, float angle, float speed, float damage, float distance) {
        entt::entity bullet = getWorld()->create();
        getWorld()->assign<Name>(bullet, "bullet");
        auto data = BulletData{.firer_id =  getWorld()->get<AgentData>(firer).player,
                .type =  type,
                .damage =  damage,
                .density = 7.6,
                .radius = 0.3,
        };
        if(type == BulletType::SHOTGUN_SHELL)
            data.radius = 0.2;
        getWorld()->assign<BulletData>(bullet, data);
        getWorld()->assign<Hitbox>(bullet, Hitbox{.radius =  data.radius});

        vec2 ang(cos(angle), sin(angle));

        getWorld()->assign<Velocity>(bullet, speed * ang);
        getWorld()->assign<Position>(bullet, getWorld()->get<Position>(firer) + ang * distance);
        getWorld()->assign<Lifespan>(bullet, lifespan->period(3));
    }

    void BulletSystem::update(float delta) {
        getWorld()->view<BulletData, CollisionResults>().each(
                [&](entt::entity ent, auto &bullet, auto &col) {
                    bool hit = false;
                    for (Collision &c : col.results) {
                        if(getWorld()->valid(c.hit_with))
                        {
                            if(getWorld()->has<BulletData>(c.hit_with))
                                continue;
                            hit = true;
                            if (getWorld()->has<Health>(c.hit_with)) {
                                if (getWorld()->has<AgentData>(c.hit_with) &&
                                    getWorld()->get<AgentData>(c.hit_with).player == bullet.firer_id) // Not do harm the firer/group
                                    continue;
                                auto &health = getWorld()->get<Health>(c.hit_with);
                                health.health -= bullet.damage;
                            }
                            break;
                        }
                    }
                    if (hit)
                        getWorld()->destroy(ent);
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

    void WeaponSystem::fire(entt::entity ent, float angle) {
        if (getWorld()->has<Weapon>(ent)) {
            auto &weapon = getWorld()->get<Weapon>(ent);
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

    void WeaponSystem::changeWeapon(entt::entity ent, WeaponType type) {
        // FIXME By changing weapon quickly, the player has a change of shooting each frame
        if (getWorld()->has<Weapon>(ent)) {
            auto &w = getWorld()->get<Weapon>(ent);
            if (type != w.weapon)
                getWorld()->assign_or_replace<Weapon>(ent, Weapon{type, 0, 0});
        } else {
            getWorld()->assign<Weapon>(ent, Weapon{type, 0});
        }
    }

    void WeaponSystem::update(float delta) {
        getWorld()->view<Message<ChangeWeapon>>().each([&](auto ent, auto &chg) {
            if (!chg.processed) {
                changeWeapon(ent, chg.data.weapon);
                chg.processed = true;
            }
        });

        getWorld()->view<Message<Shooting>>().each([&](auto ent, auto &sht) {
            if (!sht.processed) {
                fire(ent, sht.data.angle);
                sht.processed = true;
            }
        });
    }
} // namespace Escape