#if !defined(VERTEX_ARRAY_H)
#define VERTEX_ARRAY_H
#include "vertex_buffer.h"
#include "index_buffer.h"
#include <vector>
namespace Escape
{
namespace Render
{
    

struct VertexArrayLayoutElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    static int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return sizeof(GLfloat);
        case GL_UNSIGNED_INT:
            return sizeof(GLuint);
        case GL_UNSIGNED_BYTE:
            return sizeof(GLbyte);
        default:
            assert(false);
        }
    }
};
class VertexArrayLayout
{
    std::vector<VertexArrayLayoutElement> layout;
    unsigned int stride;

public:
    VertexArrayLayout() : stride(0) {}
    const std::vector<VertexArrayLayoutElement> &getElements() const
    {
        return layout;
    }
    template <typename T>
    void push(unsigned int count);
    unsigned int getStride() const
    {
        return stride;
    }
};
template <>
void VertexArrayLayout::push<float>(unsigned int count)
{
    layout.push_back({GL_FLOAT, count, GL_FALSE});
    stride += count * VertexArrayLayoutElement::getSizeOfType(GL_FLOAT);
}
template <>
void VertexArrayLayout::push<unsigned int>(unsigned int count)
{
    layout.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    stride += count * VertexArrayLayoutElement::getSizeOfType(GL_UNSIGNED_INT);
}
template <>
void VertexArrayLayout::push<char>(unsigned int count)
{
    layout.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    stride += count * VertexArrayLayoutElement::getSizeOfType(GL_UNSIGNED_BYTE);
}
class VertexArray
{
    unsigned int VAO;
    VertexArrayLayout layout;

public:
    VertexArray()
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }
    void bind(const VertexBuffer &vb, const IndexBuffer &eb, const VertexArrayLayout &layout)
    {
        this->layout = layout;
        bind();
        vb.bind();
        eb.bind();
        auto &&elements = layout.getElements();
        char *offset = 0;
        for (size_t i = 0; i < elements.size(); i++)
        {
            auto &&ele = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, ele.count, ele.type, ele.normalized, layout.getStride(), offset);
            offset += ele.count * VertexArrayLayoutElement::getSizeOfType(ele.type);
        }
        unbind();
    }
    void bind() const
    {
        glBindVertexArray(VAO);
    }
    void unbind() const
    {
        glBindVertexArray(0);
    }
    ~VertexArray()
    {
        glDeleteVertexArrays(1, &VAO);
    }
};
} // namespace Render
} // namespace Escape

#endif // VERTEX_ARRAY_H
