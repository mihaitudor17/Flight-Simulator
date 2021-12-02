
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include "Skybox.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Camera* handler = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (handler)
        handler->MouseControl((float)xpos, (float)ypos);
}

/// Initializeaza fereastra si face un contex OpenGL in aceasta fereastra
void InitWindow(GLFWwindow* (&window), const std::string& title)
{
    if (!glfwInit())
    {
        std::cerr << "GLFW failed to initialize!" << std::endl;
        exit(-1);
    }

    window = glfwCreateWindow(800, 600, title.c_str(), NULL, NULL);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW failed to initialize!" << std::endl;
        exit(-1);
    }

    glfwSwapInterval(1);       

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
/// functia care deseneaza
void RenderFunction(GLFWwindow* window)
{
    std::vector<std::string> Images
    {
            "../PictureSkybox/right.jpg",
            "../PictureSkybox/left.jpg",
            "../PictureSkybox/top.jpg",
            "..//PictureSkybox/bottom.jpg",
            "../PictureSkybox/front.jpg",
            "../PictureSkybox/back.jpg"
    };
    Skybox skybox(Images);
    Camera* camera = new Camera(800, 600, glm::vec3(0, 0, 0));
    glfwSetWindowUserPointer(window, reinterpret_cast<void*>(camera));
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        skybox.Draw(camera);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main()
{
    GLFWwindow* window;
    InitWindow(window, "Fereastra");
    RenderFunction(window);
}

