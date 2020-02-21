#if !defined(RENDERER2D_H)
#define RENDERER2D_H
#include "shader.h"
#include "scene.h"
#include "sprite2d.h"
#include "utils.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
namespace Escape
{
struct RectangleData
{
    unsigned int VAO;
    VertexBuffer vbo;
    IndexBuffer ebo;
    RectangleData()
    {
        float vertices[] = {
            1, 1, 0.0f, // top right
            1, 0, 0.0f, // bottom right
            0, 0, 0.0f, // bottom left
            0, 1, 0.0f  // top left
        };
        unsigned int indices[] = {
            0, 1, 3, // first Triangle
            1, 2, 3  // second Triangle
        };
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        vbo.init(vertices, sizeof(vertices));
        ebo.init(indices, 6);


        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
    }
    ~RectangleData() {

    }
};
class Renderer2D
{
public:
    Renderer2D() : shader("assets/shaders/point.vs", "assets/shaders/color.fs")
    {
    }

    void drawRect(const Rectangle &rect)
    {
        shader.setUniform("color", rect.color);
        glm::mat4 transform = last_scene->mat * glm::translate(glm::mat4(1.0f), rect.pos) * glm::scale(glm::mat4(1.0f), {rect.size.x, rect.size.y, 1.0f});
        shader.setUniform("transform", transform);
        glBindVertexArray(rectangle.VAO);
        glDrawElements(GL_TRIANGLES, rectangle.ebo.getCount(), GL_UNSIGNED_INT, 0);
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
    RectangleData rectangle;
};

} // namespace Escape

#endif // RENDERER2D_H
