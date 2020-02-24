#if !defined(TIMESERVER_H)
#define TIMESERVER_H
#include <chrono>
#include <random>
#include <thread>
#include "components.h"
#include "engine/MyECS.h"
#include "engine/system.h"
#include "engine/utils.h"
namespace Escape
{

class TimeServer : public ECSSystem
{
    float freq, delta;
    std::default_random_engine engine;

public:
    TimeServer(float rate)
    {
        setRate(rate);
    }
    void initialize() override
    {
        ECSSystem::initialize();
        std::cerr << "World " << world << std::endl;
        setTick(0);
    }
    void setRate(float rate)
    {
        freq = rate;
        delta = 1.0f / rate;
    }
    void update(float delta) override
    {
        setTick(getTick() + 1);
        std::chrono::steady_clock::time_point next = std::chrono::steady_clock::now() + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(this->delta));

        std::this_thread::sleep_until(next);
    }
    float random(float l, float h)
    {
        std::uniform_real_distribution<double> u(l, h);
        return u(engine);
    }
    size_t getTick()
    {
        size_t tick = -1;
        world->each<Tick>([&](Entity *ent, ComponentHandle<Tick> p) {
            tick = p->tick;
        });
        if(tick == (size_t)(-1))
            throw std::runtime_error("No tick component");
        return tick;
    }
    void setTick(size_t tick)
    {
        bool find = false;
        world->each<Tick>([&](Entity *ent, ComponentHandle<Tick> p) {
            p->tick = tick;
            find = true;
        });
        if (!find)
        {
            Entity *time_ent = world->create();
            time_ent->assign<Tick>(Tick{0});
            std::cerr << "Set tick to zero" << std::endl;
        }
    }
    clock_type now()
    {
        return getTick() * delta;
    }
    clock_type getDelta()
    {
        return delta;
    }
};
} // namespace Escape

#endif // TIMESERVER_H
