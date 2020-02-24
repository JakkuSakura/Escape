#if !defined(TIMESERVER_H)
#define TIMESERVER_H
#include <chrono>
#include <random>
#include <thread>
#include "system.h"
#include "utils.h"
namespace Escape
{

class TimeServer : public System
{
    float freq, delta;
    std::chrono::high_resolution_clock::time_point benchmark, _now;
    size_t tick;
    std::default_random_engine engine;

public:
    TimeServer(float rate) : benchmark(std::chrono::high_resolution_clock::now()), _now(benchmark), tick(0)
    {
        setRate(rate);
    }
    void setRate(float rate)
    {
        if (tick > 0)
            std::cerr << "warning: change rate of a running server" << std::endl;
        freq = rate;
        delta = 1.0f / rate;
    }

    void update(float delta) override
    {
        if (_now == benchmark)
            _now = std::chrono::high_resolution_clock::now();
        tick += 1;
        std::chrono::high_resolution_clock::time_point next = _now + std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(std::chrono::duration<double>(this->delta));

        std::this_thread::sleep_until(next);
        _now = next;
    }
    float random(float l, float h)
    {
        std::uniform_real_distribution<double> u(l, h);
        return u(engine);
    }
    size_t getTick() const
    {
        return tick;
    }
    clock_type now() const
    {
        return std::chrono::duration<double>(_now - benchmark).count();
    }
    clock_type getDelta() const
    {
        return delta;
    }
};
} // namespace Escape

#endif // TIMESERVER_H
