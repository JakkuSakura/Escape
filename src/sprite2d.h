#if !defined(SPRITE2D_H)
#define SPRITE2D_H
#include <glm/glm.hpp>
namespace Escape
{
namespace Render
{

class Sprite2D
{
public:
    virtual ~Sprite2D() {}
};
class Rectangle
{
public:
    glm::vec3 pos;
    glm::vec2 size;
    glm::vec3 color;
    Rectangle(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec3 &color) : pos(pos), size(size), color(color)
    {
    }
    Rectangle(float x, float y, float width, float height, float r, float g, float b) : pos(x, y, 0), size(width, height), color(r, g, b)
    {
    }
};
} // namespace Render
} // namespace Escape

#endif // SPRITE2D_H
