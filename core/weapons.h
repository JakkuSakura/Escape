#if !defined(WEAPONS_H)
#define WEAPONS_H
#include "MyECS.h"
#include "psysics.h"
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
    BulletSystem();
    void initialize() override;
    void fire(entt::entity firer, BulletType type, float angle, float speed, float damage, float distance);
    using ECSSystem::getWorld;
};
class WeaponSystem : public ECSSystem
{
    BulletSystem *bullet_system;
    TimeServer *timeserver;
    std::map<WeaponType, WeaponPrototype> default_weapons;

public:
    WeaponSystem();
    void initialize() override;
    void update(float delta) override;
    void fire(entt::entity ent, float angle);

    void changeWeapon(entt::entity ent, WeaponType type);
};
} // namespace Escape

#endif // WEAPONS_H
