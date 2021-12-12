#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

#define SCR_HEIGHT 800
#define SCR_WIDTH 1200

#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include "Skybox.h"
#include "Terrain.h"
#include "PointLight.h"

Shader* ShadowMappingDepthShader = nullptr;
Shader* ShadowMappingShader = nullptr;

Terrain* terrain = nullptr;

unsigned int depthMapFBO = 0;
unsigned int depthMap = 0;

void InitDepthFBO()
{
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void InitShaders()
{
    ShadowMappingDepthShader = new Shader("../Shaders/ShadowMappingDepth.vs", "../Shaders/ShadowMappingDepth.fs");
    ShadowMappingShader = new Shader("../Shaders/ShadowMapping.vs", "../Shaders/ShadowMapping.fs");

    ShadowMappingShader->SetInt("diffuseTexture", 0);
    ShadowMappingShader->SetInt("shadowMap", 1);
}

void InitTerrain()
{
    terrain = new Terrain("../PictureAirport/Airport.jpg");
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Camera* handler = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (handler)
        handler->MouseControl((float)xpos, (float)ypos);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Camera* handler = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    //if (handler)
    //{
    //    //to do: deltatime
    //    if (action == GLFW_PRESS)
    //    {
    //        if (key == GLFW_KEY_W)
    //            handler->ProcessKeyboard(ECameraMovementType::FORWARD, 0.1);
    //        if (key == GLFW_KEY_A)
    //            handler->ProcessKeyboard(ECameraMovementType::LEFT, 0.1);
    //        if (key == GLFW_KEY_S)
    //            handler->ProcessKeyboard(ECameraMovementType::BACKWARD, 0.1);
    //        if (key == GLFW_KEY_D)
    //            handler->ProcessKeyboard(ECameraMovementType::RIGHT, 0.1);
    //    }
    //}
}

void ProcessInput(GLFWwindow* window, Camera* camera)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(ECameraMovementType::FORWARD, 0.01);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(ECameraMovementType::LEFT, 0.01);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
       camera->ProcessKeyboard(ECameraMovementType::BACKWARD, 0.01);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(ECameraMovementType::RIGHT, 0.01);
}

/// Initializeaza fereastra si face un contex OpenGL in aceasta fereastra
void InitWindow(GLFWwindow* (&window), const std::string& title)
{
    if (!glfwInit())
    {
        std::cerr << "GLFW failed to initialize!" << std::endl;
        exit(-1);
    }

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title.c_str(), NULL, NULL);

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
    glfwSetKeyCallback(window, keyboard_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void RenderScene(const Shader* shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    shader->Use();
    shader->SetMat4("model", model);
    terrain->Render();

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
    PointLight light({-2.0f, 200.0f, -1.0f});
    Camera* camera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0, 0, 0));
    glfwSetWindowUserPointer(window, reinterpret_cast<void*>(camera));
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_CULL_FACE);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);




        // render scene from light's point of view
        ShadowMappingDepthShader->Use();
        ShadowMappingDepthShader->SetMat4("lightSpaceMatrix", light.LightSpaceMatrix());

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

        glClear(GL_DEPTH_BUFFER_BIT);

        //glActiveTexture(GL_TEXTURE0);

        //glBindTexture(GL_TEXTURE_2D, floorTexture);

        glEnable(GL_CULL_FACE);

        glCullFace(GL_FRONT);


        RenderScene(ShadowMappingDepthShader);

        glCullFace(GL_BACK);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2. render scene as normal using the generated depth/shadow map 
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ShadowMappingShader->Use();
        glm::mat4 projection = camera->GetProjectionMatrix();
        glm::mat4 view = camera->GetViewMatrix();
        ShadowMappingShader->SetMat4("projection", projection);
        ShadowMappingShader->SetMat4("view", view);
        // set light uniforms

        ShadowMappingShader->SetVec3("viewPos", camera->GetPosition());
        ShadowMappingShader->SetVec3("lightPos", light.GetPosition());
        ShadowMappingShader->SetMat4("lightSpaceMatrix", light.LightSpaceMatrix());







       // glActiveTexture(GL_TEXTURE0);
       // glBindTexture(GL_TEXTURE_2D, floorTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glDisable(GL_CULL_FACE);
        
        glDepthMask(GL_FALSE);
        skybox.Draw(camera);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        RenderScene(ShadowMappingShader);


        glEnable(GL_DEPTH_TEST);



        glfwSwapBuffers(window);
        glfwPollEvents();
        ProcessInput(window, camera);
    }
}

int main()
{
    GLFWwindow* window;
    InitWindow(window, "Fereastra");
    InitShaders();
    InitDepthFBO();
    InitTerrain();
    RenderFunction(window);
}

