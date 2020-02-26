#if !defined(MOVEMENT_H)
#define MOVEMENT_H
#include "MyECS.h"

#include "components.h"

namespace Escape
{
class MovementSystem : public ECSSystem
{
public:
    MovementSystem();
    void update(clock_type delta) override;

};

} // namespace Escape

#endif // MOVEMENT_H
