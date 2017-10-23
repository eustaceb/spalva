#include "AdjustableScene.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Definitions.h"
#include "../GUI.h"

#include "../resourcing/ResourceManager.h"

#include "../renderables/Renderable.h"
#include "../resourcing/Shader.h"

#include "../imgui/imgui.h"
#include "../imgui/Imgui_Impl.h"

#include "../renderables/Cube.h"
#include "../renderables/LightCube.h"
#include "../renderables/Plane.h"
#include "../renderables/Model.h"

AdjustableScene::AdjustableScene(std::shared_ptr<Camera> camera, const std::string & name)
    : Scene(camera, name)
{
    m_Shaders.insert(std::make_pair("default", 
        std::make_shared<Shader>("shaders/lampShader.vert", "shaders/lampShader.frag")));
    m_Shaders.insert(std::make_pair("model", 
        std::make_shared<Shader>("shaders/modelShader.vert", "shaders/modelShader.frag")));

    auto planeShader = std::make_shared<Shader>("shaders/plainTextured.vert", "shaders/plainTextured.frag");
    planeShader->use();
    planeShader->setInt("texture1", 0);
    m_Shaders.insert(std::make_pair("plane", planeShader));
}

AdjustableScene::~AdjustableScene()
{
}

void AdjustableScene::activate()
{
    Scene::activate();
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void AdjustableScene::deactivate()
{
    Scene::deactivate();
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glDisable(GL_DEPTH_TEST);
}

void AdjustableScene::render(glm::mat4 & projection, glm::mat4 & view)
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
}

void AdjustableScene::renderGUI()
{
    // Cube spawner window
    float spawnerWidth = 360, spawnerHeight = 300;
    ImGui::SetNextWindowSize(ImVec2(spawnerWidth, spawnerHeight), ImGuiSetCond_Once);
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - spawnerWidth - GUI::MARGIN, GUI::MARGIN), ImGuiSetCond_Once);
    ImGui::Begin("Spawn objects");

    static float cubePos[3] = { 0, 0, 0 };
    static float planePos[3] = { 0, 0, 0 };
    static float planeSize[2] = { 5.f, 5.f };
    static float modelPos[3] = { 0, 0, 0 };
    // Cube spawner
    if (ImGui::CollapsingHeader("Cube"))
    {
        ImGui::InputFloat3("Cube XYZ", &cubePos[0]);
        if (ImGui::Button("Spawn Cube", ImVec2(90, 20)))
        {
            glm::vec3 coords(cubePos[0], cubePos[1], cubePos[2]);
            auto cube = std::make_shared<LightCube>(m_Shaders.at("default"), coords, 0);
            ResourceManager::instance()->reloadActiveGroup();
            m_Renderables.push_back(cube);
        }
    }
    // Plane spawner
    if (ImGui::CollapsingHeader("Plane"))
    {
        ImGui::InputFloat3("Plane XYZ", &planePos[0]);
        ImGui::InputFloat2("Height Length", &planeSize[0], 1);
        if (ImGui::Button("Spawn Plane", ImVec2(90, 20)))
        {
            glm::vec3 coords(planePos[0], planePos[1], planePos[2]);
            auto plane = std::make_shared<Plane>(m_Shaders.at("plane"), coords,
                "textures/wood.png", planeSize[0], planeSize[1]);
            ResourceManager::instance()->reloadActiveGroup();
            m_Renderables.push_back(plane);
        }
    }
    // Model spawner
    if (ImGui::CollapsingHeader("Model"))
    {
        ImGui::InputFloat3("Model XYZ", &modelPos[0]);
        if (ImGui::Button("Spawn Model", ImVec2(90, 20)))
        {
            glm::vec3 coords(modelPos[0], modelPos[1], modelPos[2]);
            auto ourModel = std::make_shared<Model>(m_Shaders["model"], coords, 
                "models/nanosuit/nanosuit.obj");
            ourModel->scale(glm::vec3(0.2f, 0.2f, 0.2f));
            ResourceManager::instance()->reloadActiveGroup();
            m_Renderables.push_back(ourModel);
        }
    }
    // Object list
    if (m_Renderables.size() > 0)
    {
        if (ImGui::CollapsingHeader("Object List"))
        {
            ImGui::BeginGroup();
            for (auto it = m_Renderables.begin(); it != m_Renderables.end(); it++)
            {
                ImGui::Text((*it)->getLabel().c_str());
                ImGui::SameLine();
                //ImGui::SmallButton("Edit");
                ImGui::SameLine();
                if (ImGui::SmallButton("Remove")) m_Renderables.erase(it);
            }
            ImGui::EndGroup();
        }
        // Clear button
        if (ImGui::Button("Clear scene", ImVec2(100, 20))) m_Renderables.clear();
    }
    ImGui::End();
}
