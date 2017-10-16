#include <iostream>
#include <cmath>
#include <memory>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Own classes
#include "Definitions.h"

#include "Camera.h"
#include "Renderer.h"

#include "scenes/BasicScene.h"
#include "scenes/AdjustableScene.h"
#include "scenes/SkyboxScene.h"
#include "scenes/InstancingScene.h"
#include "scenes/AdvancedLightningScene.h"
#include "scenes/ParticlesScene.h"

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void doMovement(GLfloat dt);

std::shared_ptr<Camera> camera;
Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT, "Renderer");

bool firstMouse = true;
GLfloat lastX = 400, lastY = 300;
bool keys[1024];

int main()
{
    camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
    // Window setup
    GLFWwindow* window = renderer.getWindow();

    GLint windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);
    
    std::shared_ptr<Scene> scene = std::make_shared<BasicScene>(camera, "Basic Scene");
    renderer.addScene(scene);
    renderer.setScene("Basic Scene");
    std::shared_ptr<Scene> adjustableScene = std::make_shared<AdjustableScene>(camera, "Adjustable Scene");
    renderer.addScene(adjustableScene);
    std::shared_ptr<Scene> skyboxScene = std::make_shared<SkyboxScene>(camera, "Skybox Scene");
    renderer.addScene(skyboxScene);
    std::shared_ptr<Scene> instancingScene = std::make_shared<InstancingScene>(camera, "Instancing Scene");
    renderer.addScene(instancingScene);
    std::shared_ptr<Scene> advancedLightningScene = std::make_shared<AdvancedLightningScene>(camera, "Advanced Lightning Scene");
    renderer.addScene(advancedLightningScene);
    std::shared_ptr<Scene> particlesScene = std::make_shared<ParticlesScene>(camera, "Particles Scene");
    renderer.addScene(particlesScene);

    GLfloat lastFrame = 1.0f, deltaTime;
    GLfloat aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
    glm::mat4 projection, view;

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check for events, move camera and update current scene
        glfwPollEvents();
        doMovement(deltaTime);
        renderer.update(deltaTime);

        // Generate view and projection matrices
        view = camera->getViewMatrix();
        projection = glm::perspective(camera->zoom, aspectRatio, 0.1f, 100.0f);

        renderer.render(projection, view);
    }
    return 0;
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera->processMouseScroll(yOffset);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    GLfloat xOffset = xpos - lastX, yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera->processMouseMovement(xOffset, yOffset);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if ((key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) && action == GLFW_PRESS)
    {
        renderer.toggleUI();
        camera->toggleControls();
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        renderer.toggleWireframe();
    if (action == GLFW_PRESS)
        keys[key] = true;
    if (action == GLFW_RELEASE)
        keys[key] = false;
}
void doMovement(GLfloat dt)
{
    if (keys[GLFW_KEY_W])
        camera->processKeyboard(FORWARD, dt);
    if (keys[GLFW_KEY_S])
        camera->processKeyboard(BACKWARD, dt);
    if (keys[GLFW_KEY_A])
        camera->processKeyboard(LEFT, dt);
    if (keys[GLFW_KEY_D])
        camera->processKeyboard(RIGHT, dt);
}