//
// Created by jack on 20-2-25.
//
#include <chrono>
#include <random>
#include <thread>
#include "components.h"
#include "MyECS.h"
#include "engine/system.h"
#include "engine/utils.h"
#include "timeserver.h"

namespace Escape {
    TimeServer::TimeServer(float rate) {
        setRate(rate);
    }

    void TimeServer::initialize() {
        configure();
        setTick(0);
    }

    void TimeServer::setRate(float rate) {
        freq = rate;
        delta = 1.0f / rate;
    }

    void TimeServer::update(float delta) {
        using namespace std::chrono_literals;
        setTick(getTick() + 1);
        clock_type::time_point next =
                last + std::chrono::duration_cast<clock_type::duration>(std::chrono::duration<double>(this->delta));

        std::this_thread::sleep_until(next);
        last = next;
    }

    float TimeServer::random(float l, float h) {
        std::uniform_real_distribution<double> u(l, h);
        return u(engine);
    }

    size_t TimeServer::getTick() {
        size_t tick = -1;
        world->view<TimeServerInfo>().each([&](auto ent, auto &p) {
            tick = p.tick;
        });
        if (tick == (size_t) (-1))
        {
            tick = 0;
            setTick(0);
            // throw std::runtime_error("No tick component");
        }
        return tick;
    }

    void TimeServer::setTick(size_t tick) {
        bool find = false;
        world->view<TimeServerInfo>().each([&](auto ent, auto &p) {
            p.tick = tick;
            find = true;
        });
        if (!find) {
            auto time_ent = world->create();
            world->assign<TimeServerInfo>(time_ent, TimeServerInfo{tick: 0});
            std::cerr << "Set tick to zero" << std::endl;
        }
    }

    ::clock_type TimeServer::now() {
        return getTick() * delta;
    }

    ::clock_type TimeServer::getDelta() {
        return delta;
    }

    void FPSCounter::update(::clock_type delta) {
        using namespace std::chrono_literals;
        count += 1;
        auto duration = clock_type::now() - last;
        // assuming that the duration relatively equals 1 second
        if (duration > 1s) {
            std::cerr << "FPS: " << count << std::endl;
            initialize();
        }

    }

    void FPSCounter::initialize() {
        last = clock_type::now();
        count = 0;
    }

}
