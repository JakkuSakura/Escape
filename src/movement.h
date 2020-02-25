#if !defined(MOVEMENT_H)
#define MOVEMENT_H
#include "MyECS.h"
#include <glm/glm.hpp>
#include "components.h"

namespace Escape
{
class MovementSystem : public ECSSystem
{
public:
    MovementSystem();
    void move(Entity ent, const glm::vec2 &speed);
    void update(clock_type delta) override;

};

} // namespace Escape

#endif // MOVEMENT_H
