#include "utils.h"
#include <chrono>
clock_type game_clock()
{
    static auto benchmark = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    auto duration_in_milliseconds = std::chrono::duration<double, std::milli>(now - benchmark);
    double temp = duration_in_milliseconds.count();
    return temp / 1000;
}