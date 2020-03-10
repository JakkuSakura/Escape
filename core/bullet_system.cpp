//
// Created by jack on 20-3-4.
//
#include "bullet_system.h"
#include "event_system.h"

namespace Escape{
    void on_hit(BulletSystem *bulletSystem, Collision col) {
        World *world = bulletSystem->getWorld();
        if (!world->valid(col.a) || !world->valid(col.b))
            return;

        if (world->has<BulletData>(col.b))
            std::swap(col.a, col.b);
        auto *bullet = world->try_get<BulletData>(col.a);
        if (bullet)
        {
            if (world->has<TerrainData>(col.b) && world->get<TerrainData>(col.b).type == TerrainType::WATER) {
                return;
            }
            if (!world->has<BulletData>(col.b)) {
    //            if(world->valid((entt::entity)bullet->firer_id))
    //            {
    //                auto *firer = world->try_get<AgentData>((entt::entity)bullet->firer_id), *b = world->try_get<AgentData>(col.b);
    //                if (firer && b) {
    //                    if (firer->group == b->group)
    //                        goto destory;
    //                }
    //            }
                if(world->has<Health>(col.b))
                    world->get<Health>(col.b).health -= bullet->damage;
                destory:;
                world->destroy(col.a);
            }
        }
    }

    BulletSystem::BulletSystem() {

    }

    void BulletSystem::initialize() {
        ECSSystem::initialize();
        lifespan = findSystem<LifespanSystem>();

        findSystem<EventSystem>()->listen(on_hit, this);

    }

    void
    BulletSystem::fire(entt::entity firer, BulletType type, float angle, float speed, float damage, float distance) {
        entt::entity bullet = getWorld()->create();
        getWorld()->assign<Name>(bullet, "bullet");
        auto data = BulletData{.firer_id = entt::to_integral(firer),
                .type =  type,
                .damage =  damage,
                .density = 7.6,
                .radius = 0.3,
        };
        if (type == BulletType::SHOTGUN_SHELL)
            data.radius = 0.2;
        getWorld()->assign<BulletData>(bullet, data);
        getWorld()->assign<CircleShape>(bullet, data.radius);

        vec2 ang(cos(angle), sin(angle));

        getWorld()->assign<Velocity>(bullet, speed * ang);
        getWorld()->assign<Position>(bullet, getWorld()->get<Position>(firer) + ang * distance);
        getWorld()->assign<Lifespan>(bullet, lifespan->period(3));
    }
}
