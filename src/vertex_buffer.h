#if !defined(VERTEX_BUFFER_H)
#define VERTEX_BUFFER_H
#include <GL/glew.h>
#include <cassert>
namespace Escape
{
namespace Render
{

class VertexBuffer
{
    unsigned int VBO;

public:
    VertexBuffer() : VBO(0) {}

    VertexBuffer(void *data, size_t size)
    {
        init(data, size);
    }
    void init(void *data, size_t size)
    {
        assert(VBO == 0);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
    ~VertexBuffer()
    {
        if (VBO)
            glDeleteBuffers(1, &VBO);
    }
    void bind() const
    {
        assert(VBO != 0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }
    void unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
} // namespace Render
} // namespace Escape

#endif // VERTEX_BUFFER_H
