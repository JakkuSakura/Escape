#if !defined(SCENE_H)
#define SCENE_H
#include "glm/glm.hpp"
#include "shader.h"
namespace Escape
{
namespace Render
{

class Scene
{
public:
    glm::mat4 mat;
    Scene(const glm::mat4 &m) : mat(m)
    {
    }
    Scene() : Scene(glm::mat4(1.0f))
    {
    }
    virtual ~Scene() {}
    virtual void apply(Shader &shader)
    {
        shader.setUniform("transform", mat);
        shader.use();
    };
};
} // namespace Render
} // namespace Escape

#endif // SCENE_H
