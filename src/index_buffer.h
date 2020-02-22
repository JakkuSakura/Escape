#if !defined(INDEX_BUFFER_H)
#define INDEX_BUFFER_H
#include <GL/glew.h>
#include <cassert>
namespace Escape
{
namespace Render
{

class IndexBuffer
{
    unsigned int EBO, count;

public:
    IndexBuffer() : EBO(0), count(0)

    {
    }
    IndexBuffer(unsigned int *data, unsigned int count)
    {
        init(data, count);
    }
    void init(unsigned int *data, unsigned int count)
    {
        assert(sizeof(unsigned int) == sizeof(GLuint));
        this->count = count;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }
    ~IndexBuffer()
    {
        if (EBO)
            glDeleteBuffers(1, &EBO);
    }
    int getCount() const
    {
        return count;
    }
    void bind() const
    {
        assert(EBO != 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }
    void unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};
} // namespace Render
} // namespace Escape

#endif // INDEX_BUFFER_H
