#if !defined(TIMESERVER_H)
#define TIMESERVER_H
#include <chrono>
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

public:
    TimeServer(float rate) : freq(rate), delta(1.0f / rate), benchmark(std::chrono::high_resolution_clock::now()), _now(benchmark), tick(0)
    {
    }
    
    void update(float delta) override
    {
        if(_now == benchmark)
            _now = std::chrono::high_resolution_clock::now();
        tick += 1;
        std::chrono::high_resolution_clock::time_point next = _now + std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(std::chrono::duration<double>(this->delta));

        std::this_thread::sleep_until(next);
        _now = next;
    }
    size_t getTick() const {
        return tick;
    }
    clock_type now() const {
        return std::chrono::duration<double>(_now - benchmark).count();
    }
    clock_type getDelta() const
    {
        return delta;
    } 
};
} // namespace Escape

#endif // TIMESERVER_H
