#if !defined(TIMESERVER_H)
#define TIMESERVER_H
#include <random>
#include "MyECS.h"
#include "engine/utils.h"
namespace Escape
{

class TimeServer : public ECSSystem
{
    float freq, delta;
    std::default_random_engine engine;

public:
    TimeServer(float rate);
    void initialize() override;
    void setRate(float rate);
    void update(float delta) override;
    float random(float l, float h);
    size_t getTick();
    void setTick(size_t tick);
    clock_type now();
    clock_type getDelta();
};
} // namespace Escape

#endif // TIMESERVER_H
