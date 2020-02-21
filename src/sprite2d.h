#if !defined(SPRITE2D_H)
#define SPRITE2D_H

namespace Escape
{
class Sprite2D
{
public:
    virtual void draw() = 0;
    virtual ~Sprite2D() {}
};
class Rectangle : public Sprite2D
{
    unsigned int VBO, VAO, EBO;
    float x, y, width, height;
    float r, g, b;

public:
    Rectangle(float x, float y, float width, float height, float r, float g, float b) : x(x), y(y), width(width), height(height), r(r), g(g), b(b)
    {
        float vertices[] = {
            x + width, y + height, 0.0f, r, g, b, // top right
            x + width, y, 0.0f, r, g, b,          // bottom right
            x, y, 0.0f, r, g, b,                  // bottom left
            x, y + height, 0.0f, r, g, b          // top left
        };
        unsigned int indices[] = {
            0, 1, 3, // first Triangle
            1, 2, 3  // second Triangle
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    void draw() override
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
};
} // namespace Escape

#endif // SPRITE2D_H
