#if !defined(LIFESPAN_H)
#define LIFESPAN_H
#include "timeserver.h"
#include "components.h"
namespace Escape
{

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
        world->view<Lifespan>().each([&](Entity ent, auto &lifespan) {
            if (lifespan.end < now)
            {
                world->destroy(ent);
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
