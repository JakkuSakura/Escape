#if !defined(LIFESPAN_H)
#define LIFESPAN_H

namespace Escape
{
struct Lifespan
{
    clock_type begin;
    clock_type end;
};
class LifespanSystem : public ECSSystem
{
public:
    void update(clock_type delta) override
    {
        clock_type now = game_clock();
        world->each<Lifespan>([&](Entity *ent, ComponentHandle<Lifespan> lifespan) {
            if (lifespan->end < now)
            {
                world->destroy(ent, false);
            }
        });
    }
};
} // namespace Escape

#endif // LIFESPAN_H
