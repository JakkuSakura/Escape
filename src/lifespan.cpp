//
// Created by jack on 20-2-25.
//
#include "lifespan.h"
void Escape::LifespanSystem::initialize() {
    ECSSystem::initialize();
    timeserver = findSystem<TimeServer>();
}

void Escape::LifespanSystem::update(clock_type delta) {
    clock_type now = timeserver->now();
    world->view<Lifespan>().each([&](Entity ent, auto &lifespan) {
        if (lifespan.end < now)
        {
            world->destroy(ent);
        }
    });
}

Lifespan Escape::LifespanSystem::period(float secs) {
    return Lifespan{.begin =  timeserver->now(), .end =  timeserver->now() + secs};
}
