#if !defined(MOVEMENT_H)
#define MOVEMENT_H
#include "engine/MyECS.h"
#include <glm/glm.hpp>

namespace Escape
{
struct Position : public glm::vec2
{
    using glm::vec2::vec2;
    glm::vec2 &as_vec2()
    {
        return *this;
    }
};
struct Velocity : public glm::vec2
{
    using glm::vec2::vec2;
    glm::vec2 &as_vec2()
    {
        return *this;
    }
};
struct Hitbox {
    float radius;
};
class MovementSystem : public ECSSystem
{
public:
    void move(Entity *ent, const glm::vec2 &speed)
    {
        if (speed.x == 0 && speed.y == 0)
            ent->remove<Velocity>();
        ent->assign<Velocity>(speed.x, speed.y);
    }
    void update(clock_type delta) override
    {
        world->each<Velocity>([&](Entity *ent, ComponentHandle<Velocity> vel) {
            auto &&pos = ent->get<Position>();
            assert(pos.isValid());
            pos.get() += vel.get() * delta;
        });
    }

private:
};

} // namespace Escape


#endif // MOVEMENT_H
