#if !defined(TIMESERVER_H)
#define TIMESERVER_H

#include <random>
#include <chrono>
#include "MyECS.h"
#include "engine/utils.h"

namespace Escape {
    class FPSCounter : public System {
        typedef std::chrono::steady_clock clock_type;
        clock_type::time_point last;
        int count;
    public:
        void initialize() override;
        void update(::clock_type delta) override;


    };

    class TimeServer : public ECSSystem {
        typedef std::chrono::steady_clock clock_type;
        float freq, delta;
        clock_type::time_point last;
        std::default_random_engine engine;

    public:
        TimeServer(float rate);

        void initialize() override;

        void setRate(float rate);

        void update(float delta) override;

        float random(float l, float h);

        size_t getTick();

        void setTick(size_t tick);

        ::clock_type now();

        ::clock_type getDelta();
    };
} // namespace Escape

#endif // TIMESERVER_H
