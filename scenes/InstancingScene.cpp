#include "InstancingScene.h"

#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "../renderables/Model.h"
#include "../renderables/InstancedModel.h"
#include "../renderables/Skybox.h"

#include "../imgui/imgui.h"
#include "../imgui/Imgui_Impl.h"
#include "../Definitions.h"
#include "../GUI.h"

InstancingScene::InstancingScene(std::shared_ptr<Camera> camera, const std::string & name)
    : Scene(camera, name)
{
    auto planetShader = std::make_shared<Shader>("shaders/modelShader.vert", "shaders/modelShader.frag");
    m_Shaders.insert(std::make_pair(std::string("PlanetShader"), planetShader));

    auto planet = std::make_shared<Model>(planetShader, glm::vec3(0.0f, -3.0f, Z_DISPLACEMENT), "models/planet/planet.obj", "Planet");
    planet->scale(glm::vec3(3.0f, 3.0f, 3.0f));
    m_Renderables.push_back(planet);
    
    auto asteroidShader = std::make_shared<Shader>("shaders/asteroidShader.vert", "shaders/modelShader.frag");
    asteroidShader->setInt("material.texture_diffuse1", 0);
    m_Shaders.insert(std::make_pair(std::string("AsteroidShader"), asteroidShader));
    generateAsteroids(2000, 25.0f, 5.0f);

    // Setup skybox
    auto skyboxShader = std::make_shared<Shader>("shaders/skybox.vert", "shaders/skybox.frag");
    skyboxShader->use();
    skyboxShader->setInt("skybox", 0);

    m_Shaders.insert(std::make_pair("skybox", skyboxShader));

    std::vector<std::string> faces
    {
        "textures/skybox/bkg1_right1.png", // right
        "textures/skybox/bkg1_left2.png", // left
        "textures/skybox/bkg1_top3.png", // up
        "textures/skybox/bkg1_bottom4.png", // down
        "textures/skybox/bkg1_front5.png", // back
        "textures/skybox/bkg1_back6.png"  // front
    };

    m_Skybox = std::make_unique<Skybox>(skyboxShader, glm::vec3(0.f), faces);
}


InstancingScene::~InstancingScene()
{
}

void InstancingScene::activate()
{
    Scene::activate();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glClearColor(0.2f, 0.2f, 0.0f, 1.0f);
}

void InstancingScene::deactivate()
{
    Scene::deactivate();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
}

void InstancingScene::render(glm::mat4 & projection, glm::mat4 & view)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto it = m_Renderables.begin(); it != m_Renderables.end(); it++)
    {
        auto currentShader = (*it)->getShader();
        currentShader->use();
        currentShader->setMat4("view", view);
        currentShader->setMat4("projection", projection);

        // if has model in current shader
        if ((*it)->getLabel() != "Asteroid")
        {
            glm::mat4 model = (*it)->getModelMatrix();
            currentShader->setMat4("model", model);
        }
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

void InstancingScene::renderGUI()
{
    float width = 300, height = 100;

    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_Once);
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - width - GUI::MARGIN, GUI::MARGIN), ImGuiSetCond_Once);

    ImGui::Begin("Asteroid configuration");
    static int count = 2000;
    static float radius = 25.0f, offset = 5.0f;
    if (ImGui::SliderInt("Count", &count, 1, 25000)) generateAsteroids(count, radius, offset);
    if (ImGui::SliderFloat("Radius", &radius, 0.0f, 90.0f)) generateAsteroids(count, radius, offset);
    if (ImGui::SliderFloat("Dispersion", &offset, 0.01f, 100.0f)) generateAsteroids(count, radius, offset);
    ImGui::End();
}

void InstancingScene::generateAsteroids(const unsigned int & count, const float & radius, const float & offset)
{
    if (count == 0 || offset == 0.0f)
    {
        std::cout << "ERR::InstancingScene::" << "Asteroid count/offset can't be 0.\n";
        return;
    }
    std::vector<glm::mat4> modelMatrices;
    srand(glfwGetTime()); // initialize random seed	
    for (unsigned int i = 0; i < count; i++)
    {
        glm::mat4 model;
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)count * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement + Z_DISPLACEMENT;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices.push_back(model);
    }
    // Remove old asteroid
    for (auto it = m_Renderables.begin(); it != m_Renderables.end(); it++)
    {
        if ((*it)->getLabel() == "Asteroid")
        {
            m_Renderables.erase(it);
            break;
        }
    }
    // And create the new one
    auto asteroid = std::make_shared<InstancedModel>(m_Shaders["AsteroidShader"],
        glm::vec3(0, 0, 0), "models/rock/rock.obj", count, modelMatrices, "Asteroid");
    m_Renderables.push_back(asteroid);
}
