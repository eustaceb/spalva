#include "BasicScene.h"
#include <glad/glad.h>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../renderables/LightModel.h"
#include "../renderables/LightCube.h"
#include "../renderables/Skybox.h"
#include "../Camera.h"

#include "../imgui/imgui.h"
#include "../imgui/Imgui_Impl.h"
#include "../Definitions.h"
#include "../GUI.h"


BasicScene::BasicScene(std::shared_ptr<Camera> camera)
    : Scene(camera)
{
    // Setup model
    auto shader = std::make_shared<Shader>("shaders/nanoShader.vert", "shaders/nanoShader.frag");
    m_Shaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(std::string("NanoShader"), shader));

    auto ourModel = std::make_shared<LightModel>(shader, glm::vec3(0.0f, -1.75f, -1.0f), "models/nanosuit/nanosuit.obj", m_Camera);
    m_Renderables.push_back(ourModel);

    ourModel->scale(glm::vec3(0.2f, 0.2f, 0.2f));

    // Setup lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    auto lightCubeShader = std::make_shared<Shader>("shaders/lampShader.vert", "shaders/lampShader.frag");
    m_Shaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(std::string("LightCubeShader"), lightCubeShader));
    
    for (GLuint i = 0; i < 4; i++)
    {
        // Create light cube
        auto lightCube = std::make_shared<LightCube>(lightCubeShader, pointLightPositions[i], i);
        lightCube->setLabel("LightCube");
        lightCube->setAmbient(glm::vec3(0.1f));
        lightCube->setSpecular(glm::vec3(1.f, 1.0f, 1.0f));
        lightCube->setDiffuse(glm::vec3(0.8f, 0.8f, 0.8f));
        lightCube->setConstant(1.0f);
        lightCube->setLinear(0.09f);
        lightCube->setQuadratic(0.032f);
        lightCube->scale(glm::vec3(0.2f));
        m_Renderables.push_back(lightCube);
        ourModel->addPointLight(lightCube, i);
        lightCube->addObserver(ourModel);
    }

    // Setup skybox
    auto skyboxShader = std::make_shared<Shader>("shaders/skybox.vert", "shaders/skybox.frag");
    skyboxShader->use();
    skyboxShader->setInt("skybox", 0);

    m_Shaders.insert(std::make_pair("skybox", skyboxShader));

    std::vector<std::string> faces
    {
        "textures/skybox/FullMoonRight2048.png", // right
        "textures/skybox/FullMoonLeft2048.png", // left
        "textures/skybox/FullMoonUp2048.png", // up
        "textures/skybox/FullMoonDown2048.png", // down
        "textures/skybox/FullMoonBack2048.png", // back
        "textures/skybox/FullMoonFront2048.png"  // front
    };

    m_Skybox = std::make_unique<Skybox>(skyboxShader, glm::vec3(0.f), faces);
}

void BasicScene::activate()
{
    Scene::activate();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void BasicScene::deactivate()
{
    Scene::deactivate();
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
}

void BasicScene::render(glm::mat4 & projection, glm::mat4 & view)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto it = m_Renderables.begin(); it != m_Renderables.end(); it++)
    {
        glm::mat4 model = (*it)->getModelMatrix();

        auto currentShader = (*it)->getShader();
        currentShader->use();
        currentShader->setMat4("model", model);
        currentShader->setMat4("view", view);
        currentShader->setMat4("projection", projection);

        (*it)->render();
    }
    auto skyboxShader = m_Skybox->getShader();
    skyboxShader->use();
    skyboxShader->setMat4("view", glm::mat4(glm::mat3(view)));
    skyboxShader->setMat4("projection", projection);

    glDepthFunc(GL_LEQUAL);
    m_Skybox->render();
    glDepthFunc(GL_LESS);
}

void BasicScene::renderGUI()
{
    // Lights manager
    int width = 250, height = 180;

    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_Once);
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - width - GUI::MARGIN, GUI::MARGIN), ImGuiSetCond_Once);
    ImGui::Begin("Customize light colors");

    {
        static float ambient[3] = {0.05, 0.05, 0.05}, diffuse[3] = {0.8, 0.8, 0.8}, specular[3] = {1, 1, 1},
            constant = 1.f, linear = 0.09f, quadratic = 0.032f;

        bool ambientField = ImGui::ColorEdit3("Ambient", &ambient[0]),
            diffuseField = ImGui::ColorEdit3("Diffuse", &diffuse[0]),
            specularField = ImGui::ColorEdit3("Specular", &specular[0]),
            constantField = ImGui::SliderFloat("Constant", &constant, 0.f, 2.f),
            linearField = ImGui::SliderFloat("Linear", &linear, 0.f, 1.f),
            quadraticField = ImGui::SliderFloat("Quadratic", &quadratic, 0.f, .3f);

        if (ambientField || diffuseField || specularField || constantField || linearField || quadraticField)
        {
            for (auto it = m_Renderables.begin(); it != m_Renderables.end(); it++)
            {
                if ((*it)->getLabel() == "LightCube")
                {
                    LightCube* light = (LightCube*)(*it).get();
                    if (ambientField) light->setAmbient(glm::vec3(ambient[0], ambient[1], ambient[2]));
                    if (diffuseField) light->setDiffuse(glm::vec3(diffuse[0], diffuse[1], diffuse[2]));
                    if (specularField) light->setSpecular(glm::vec3(specular[0], specular[1], specular[2]));
                    if (constantField) light->setConstant(constant);
                    if (linearField) light->setLinear(linear);
                    if (quadraticField) light->setQuadratic(quadratic);
                }
            }
        }
    }
    ImGui::End();
}
