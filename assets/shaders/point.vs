#version 330 core
uniform mat4 transform;
layout (location = 0) in vec3 aPos;
out vec3 ourColor;
void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
};