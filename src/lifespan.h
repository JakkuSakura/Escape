#if !defined(LIFESPAN_H)
#define LIFESPAN_H
#include "engine/timeserver.h"
namespace Escape
{
struct Lifespan
{
    clock_type begin;
    clock_type end;
};
class LifespanSystem : public ECSSystem
{
    TimeServer *timeserver;
public:
    void initialize() override
    {
        ECSSystem::initialize();
        timeserver = findSystem<TimeServer>();
    }
    void update(clock_type delta) override
    {
        clock_type now = timeserver->now();
        world->each<Lifespan>([&](Entity *ent, ComponentHandle<Lifespan> lifespan) {
            if (lifespan->end < now)
            {
                world->destroy(ent, false);
            }
        });
    }
    Lifespan period(float secs)
    {
        return Lifespan{begin : timeserver->now(), end : timeserver->now() + secs};
    }
};
} // namespace Escape

#endif // LIFESPAN_H
