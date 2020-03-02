//
// Created by jack on 20-2-25.
//
#include "lifespan.h"

namespace Escape {

    void LifespanSystem::update(clock_type delta) {
        clock_type now = timeserver->now();
        getWorld()->view<Lifespan>().each([&](entt::entity ent, auto &lifespan) {
            if (lifespan.end < now) {
                getWorld()->destroy(ent);
            }
        });
    }

    Lifespan LifespanSystem::period(float secs) {
        return Lifespan{.begin =  timeserver->now(), .end =  timeserver->now() + secs};
    }

    void LifespanSystem::initialize() {
        ECSSystem::initialize();
        timeserver = findSystem<TimeServer>();
    }

}
