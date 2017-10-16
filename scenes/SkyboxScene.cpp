#include "SkyboxScene.h"

#include <memory>
#include <glad/glad.h>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../Camera.h"
#include "../resourcing/Shader.h"
#include "../renderables/Cube.h"

#include "../imgui/imgui.h"
#include "../imgui/Imgui_Impl.h"

#include "../Definitions.h"
#include "../GUI.h"

SkyboxScene::SkyboxScene(std::shared_ptr<Camera> camera, const std::string & name)
    : Scene(camera, name)
{
    m_Camera = camera;
    auto skyboxShader = std::make_shared<Shader>("shaders/skybox.vert", "shaders/skybox.frag");
    skyboxShader->use();
    skyboxShader->setInt("skybox", 0);

    m_Shaders.insert(std::make_pair("skybox", skyboxShader));

    std::vector<std::string> faces
    {
        "textures/skybox/CloudyLightRaysRight2048.png", // right posx
        "textures/skybox/CloudyLightRaysLeft2048.png", // left negx
        "textures/skybox/CloudyLightRaysUp2048.png", // up posy
        "textures/skybox/CloudyLightRaysDown2048.png", // down negy
        "textures/skybox/CloudyLightRaysBack2048.png", // back posz
        "textures/skybox/CloudyLightRaysFront2048.png"  // front negz
    };

    m_Skybox = std::make_unique<Skybox>(skyboxShader, glm::vec3(0.f), faces);

    // Cube
    std::shared_ptr<Shader> reflectiveShader = std::make_shared<Shader>("shaders/reflective.vert", "shaders/reflective.frag");
    reflectiveShader->use();
    reflectiveShader->setInt("skybox", 0);
    reflectiveShader->setBool("mirror", GL_FALSE);
    reflectiveShader->setBool("mixColors", GL_FALSE);
    m_Shaders.insert(std::make_pair("reflective", reflectiveShader));
    m_ShaderLabels.push_back("reflective");

    std::shared_ptr<Shader> refractiveShader = std::make_shared<Shader>("shaders/reflective.vert", "shaders/refractive.frag");
    refractiveShader->use();
    refractiveShader->setInt("skybox", 0);
    refractiveShader->setFloat("ratio", 1.00f / 1.52f);
    m_Shaders.insert(std::make_pair("refractive", refractiveShader));
    m_ShaderLabels.push_back("refractive");

    auto cube = std::make_shared<Cube>(m_Shaders.at("reflective"), glm::vec3(0.f));
    m_Renderables.push_back(cube);
}


SkyboxScene::~SkyboxScene()
{
}

void SkyboxScene::activate()
{
    Scene::activate();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.2f, 0.1f, 1.0f);
}

void SkyboxScene::deactivate()
{
    Scene::deactivate();
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
}

void SkyboxScene::render(glm::mat4 & projection, glm::mat4 & view)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto it = m_Renderables.begin(); it != m_Renderables.end(); it++)
    {
        glm::mat4 model = (*it)->getModelMatrix();

        auto currentShader = (*it)->getShader();
        currentShader->use();
        currentShader->setVec3("cameraPos", m_Camera->position);
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

void SkyboxScene::renderGUI()
{
    // Object list setup
    float width = 300, height = 200;

    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_Once);
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - width - GUI::MARGIN, GUI::MARGIN), ImGuiSetCond_Once);

    ImGui::Begin("Object list");
    if (m_Renderables.size() > 0)
    {
        ImGui::BeginGroup();
        for (auto it = m_Renderables.begin(); it != m_Renderables.end(); it++)
        {
            // Object name
            ImGui::Text((*it)->getLabel().c_str());
            // Shader list
            static int index = 0;
            static int lastIndex = 0;
            const char* title = "Shader";

            ImGui::SameLine();
            ImGui::PushItemWidth(100);
            ImGui::Combo(title, &index, m_ShaderLabels);
            ImGui::PopItemWidth();
            if (index != lastIndex)
            {
                (*it)->setShader(m_Shaders.at(m_ShaderLabels[index]));
                lastIndex = index;
            }
            auto currentShader = (*it)->getShader().get();
            auto uniforms = currentShader->getUserSettableUniforms();

            GUI::renderShaderOptions(currentShader, uniforms);
        }
        ImGui::EndGroup();
    }
    else ImGui::Text("Object list is empty.");

    ImGui::End();
}
