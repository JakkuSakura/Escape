#if !defined(WEAPONS_H)
#define WEAPONS_H
#include "MyECS.h"
#include "movement.h"
#include "lifespan.h"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <map>
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
    MINIGUN_BULLET,
    RIFLE_BULLET
};
struct BulletPrototype
{
    BulletType type;
    float damage;
    float accuracy;
    float peice_number;
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
            accuracy : 10,
            peice_number : 1
        };
        default_bullets[BulletType::MINIGUN_BULLET] = BulletPrototype{
            type : BulletType::MINIGUN_BULLET,
            damage : 7,
            accuracy : 8,
            peice_number : 1
        };
        default_bullets[BulletType::SHOTGUN_SHELL] = BulletPrototype{
            type : BulletType::SHOTGUN_SHELL,
            damage : 8,
            accuracy : 3,
            peice_number : 5
        };
        default_bullets[BulletType::RIFLE_BULLET] = BulletPrototype{
            type : BulletType::RIFLE_BULLET,
            damage : 50,
            accuracy : 100,
            peice_number : 1
        };
    }
    void initialize() override
    {
        ECSSystem::initialize();
        lifespan = findSystem<LifespanSystem>();
    }
    float lookUpDamage(BulletType type) const
    {
        return default_bullets.at(type).damage;
    }
    void fire(Entity *firer, BulletType type, float angle)
    {
        Entity *bullet = world->create();
        bullet->assign<Name>("bullet");
        bullet->assign<BulletData>(BulletData{firer_id : firer->getEntityId(), type : type, damage : lookUpDamage(type), hit : false});
        float speed = 300;
        bullet->assign<Velocity>(speed * cos(angle), speed * sin(angle));
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
} // namespace Escape

#endif // WEAPONS_H
