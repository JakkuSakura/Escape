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
    void initialize() override;
    void update(clock_type delta) override;
    Lifespan period(float secs);
};
} // namespace Escape

#endif // LIFESPAN_H
