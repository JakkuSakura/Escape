#if !defined(DISPLAY_H)
#define DISPLAY_H
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
namespace Escape
{
class Display;
extern Display *instance;
class Display
{
private:
    std::string title;
    int width, height;
    GLFWwindow *window;

public:
    Display(const std::string &title, int width, int height)
    {
        assert(instance == nullptr /*You can only create one Display. Multi-window not supportted yet.*/);
        instance = this;
        this->title = title;
        this->width = width;
        this->height = height;
        initAll();
    };

    virtual void render(){};
    virtual void loop()
    {
        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            render();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    virtual void processInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
    virtual ~Display()
    {
        glfwTerminate();
    }

private:
    static void windowResizedWrapper(GLFWwindow *window, int width, int height)
    {
        instance->windowResized(window, width, height);
    }
    virtual void windowResized(GLFWwindow *window, int width, int height)
    {
        this->width = width;
        this->height = height;
        glViewport(0, 0, width, height);
    }
    void initGLFW()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (window == NULL)
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

} // namespace Escape

#endif // DISPLAY_H
