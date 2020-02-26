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
    void fire(Entity firer, BulletType type, float angle, float speed, float damage, float distance);
    void update(clock_type delta) override;
};
class WeaponSystem : public ECSSystem
{
    BulletSystem *bullet_system;
    TimeServer *timeserver;
    std::map<WeaponType, WeaponPrototype> default_weapons;

public:
    WeaponSystem();
    void initialize() override;
    void update(clock_type delta) override;
    void fire(Entity ent, float angle);

    void changeWeapon(Entity ent, WeaponType type);
};
} // namespace Escape

#endif // WEAPONS_H
