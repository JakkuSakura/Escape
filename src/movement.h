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
    void move(Entity ent, const glm::vec2 &speed)
    {
        world->assign_or_replace<Velocity>(ent, speed.x, speed.y);
    }
    void update(clock_type delta) override
    {
        world->view<Velocity, Position>().each([&](auto Ent, auto &vel, auto &pos){
            pos += vel * delta;
        });
    }

private:
};

} // namespace Escape

#endif // MOVEMENT_H
