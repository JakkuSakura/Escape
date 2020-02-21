#if !defined(RENDERER2D_H)
#define RENDERER2D_H
#include "shader.h"
#include "scene.h"
#include "sprite2d.h"
namespace Escape
{
class Renderer2D
{
public:
    Renderer2D() : shader("assets/shaders/point_and_color.vs", "assets/shaders/color.fs"),
                   rect(-1, -1, 2, 2, 1.0f, 0, 0)
    {
    }

    void drawRect()
    {
        rect.draw();
    }
    void applyShader(Shader &shader)
    {
        this->shader = shader;
        last_scene->apply(shader);
    }
    void applyScene(Scene &scene)
    {
        last_scene = &scene;
        scene.apply(shader);
    }

private:
    Shader shader;
    Scene *last_scene;

    Rectangle rect;
};

} // namespace Escape

#endif // RENDERER2D_H
