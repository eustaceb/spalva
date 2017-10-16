#include "ParticlesScene.h"

#include "../renderables/BillboardParticles.h"
#include "../resourcing/Shader.h"

#include "../imgui/imgui.h"
#include "../imgui/Imgui_Impl.h"
#include "../Definitions.h"
#include "../GUI.h"

ParticlesScene::ParticlesScene(std::shared_ptr<Camera> camera, const std::string & name)
    : Scene(camera, name)
{
    auto particleShader = std::make_shared<Shader>("shaders/particles.vert", "shaders/particles.frag");
    m_Shaders.insert(std::make_pair(std::string("Particles"), particleShader));

    m_ParticleEmitter = std::make_unique<BillboardParticles>(particleShader, glm::vec3(0.f), 0.01f, 2000);
}

ParticlesScene::~ParticlesScene()
{
}

void ParticlesScene::activate()
{
    Scene::activate();
    glEnable(GL_BLEND);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ParticlesScene::deactivate()
{
    Scene::deactivate();
    glDisable(GL_BLEND);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
}

void ParticlesScene::update(GLfloat deltaTime)
{
    m_ParticleEmitter->update(deltaTime);
}

void ParticlesScene::render(glm::mat4 & projection, glm::mat4 & view)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto currentShader = m_ParticleEmitter->getShader();
    currentShader->use();
    currentShader->setMat4("view", view);
    currentShader->setMat4("projection", projection);
    m_ParticleEmitter->render(view);
}

void ParticlesScene::renderGUI()
{
    float width = 300, height = 460;

    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_Once);
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - width - GUI::MARGIN, GUI::MARGIN), ImGuiSetCond_Once);

    ImGui::Begin("Particle spawner");

    // Defaults
    static int count = 2000, speed = 100;
    static float pos[3] = { 0, 0, 0 };
    static float size = 0.01;
    static int randomness = 1000;
    static float lifetime[2] = {1, 5};
    static float color[3] = {1.0, 0.2, 0};
    static float rangeX[2] = {-0.1, 0.1};
    static float rangeY[2] = {0, 0.4};
    static float rangeZ[2] = {-0.1, 0.1};

    ImGui::ColorPicker3("Color", &color[0], ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoInputs);
    ImGui::SliderInt("Count", &count, 1, 25000);
    ImGui::InputFloat3("Position", &pos[0]);
    ImGui::SliderFloat("Size", &size, 0.001, 0.2);
    ImGui::SliderInt("Speed", &speed, 1, 1000);
    ImGui::SliderFloat2("Lifetime", &lifetime[0], 0.1, 10);
    ImGui::InputInt("Randomness", &randomness, 1, 5000);
    ImGui::SliderFloat2("X Range", &rangeX[0], -1, 1);
    ImGui::SliderFloat2("Y Range", &rangeY[0], -1, 1);
    ImGui::SliderFloat2("Z Range", &rangeZ[0], -1, 1);

    if (ImGui::Button("Respawn", ImVec2(200, 30)))
    {
        m_ParticleEmitter = std::make_unique<BillboardParticles>(m_Shaders["Particles"], 
        glm::vec3(pos[0], pos[1], pos[2]), 
        size, count, 
        (float) speed / 1000, 
        glm::vec3(color[0], color[1], color[2]),
        glm::vec3(rangeX[0], rangeY[0], rangeZ[0]), 
        glm::vec3(rangeX[1], rangeY[1], rangeZ[1]), 
        glm::vec2(lifetime[0], lifetime[1]), randomness);
    }
    
    ImGui::End();
}
