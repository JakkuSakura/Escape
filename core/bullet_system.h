//
// Created by jack on 20-3-4.
//

#ifndef ESCAPE_BULLET_SYSTEM_H
#define ESCAPE_BULLET_SYSTEM_H

#include "lifespan.h"
namespace Escape {

    class BulletSystem : public ECSSystem
    {
        LifespanSystem *lifespan;

    public:
        BulletSystem();
        void initialize() override;
        void fire(entt::entity firer, BulletType type, float angle, float speed, float damage, float distance);
        using ECSSystem::getWorld;
    };
}
#endif //ESCAPE_BULLET_SYSTEM_H
