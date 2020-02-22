#if !defined(DISPLAY_H)
#define DISPLAY_H
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include "system.h"
#include "utils.h"
namespace Escape
{
namespace Render
{

class Window : public System
{
protected:
    std::string title;
    int width, height;
    GLFWwindow *window;
    clock_type delta;

public:
    static Window *window_instance;
    Window(const std::string &title, int width, int height)
    {
        assert(window_instance == nullptr /*You can only create one Window. Multi-window not supportted yet.*/);
        window_instance = this;
        this->title = title;
        this->width = width;
        this->height = height;
        initAll();
    };
    virtual void render(){};
    virtual void update(clock_type delta)
    {
        this->delta = delta;
        processInput(window);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    virtual bool isRunning()
    {
        return !glfwWindowShouldClose(window);
    }
    virtual void processInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
    virtual ~Window()
    {
        glfwTerminate();
        window_instance = nullptr;
    }

    virtual void windowResized(int width, int height)
    {
        this->width = width;
        this->height = height;
        glViewport(0, 0, width, height);
    }

private:
    static void windowResizedWrapper(GLFWwindow *window, int width, int height)
    {
        window_instance->windowResized(width, height);
    }
    void initGLFW()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (window == nullptr)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(-1);
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, windowResizedWrapper);
    }
    void initGLEW()
    {
        glewExperimental = GL_TRUE;
        if (glewInit())
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            exit(-1);
        }
    }
    void initOpenGL()
    {
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
    void initAll()
    {
        initGLFW();
        initGLEW();
        initOpenGL();
    }
};
} // namespace Render

} // namespace Escape

#endif // DISPLAY_H
