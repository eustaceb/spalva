#include "AdvancedLightningScene.h"
#include <GLFW/glfw3.h>

#include "../renderables/Renderable.h"
#include "../renderables/Plane.h"
#include "../renderables/Cube.h"
#include "../resourcing/Shader.h"

#include "../imgui/imgui.h"
#include "../imgui/Imgui_Impl.h"

#include "../Definitions.h"
#include "../GUI.h"

AdvancedLightningScene::AdvancedLightningScene(std::shared_ptr<Camera> camera, const std::string & name)
    : Scene(camera, name), m_LightPos(-2.0f, 4.0f, -1.0f)
{
    // Setup directional light
    m_DirLight.nearPlane = 1.0f;
    m_DirLight.farPlane = 7.5f;
    m_DirLight.lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 
        m_DirLight.nearPlane, m_DirLight.farPlane);
    m_DirLight.lightView = glm::lookAt(m_LightPos,
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // Setup depth texture
    glGenTextures(1, &m_DepthMap);
    glBindTexture(GL_TEXTURE_2D, m_DepthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_RES_W, SHADOW_RES_H,
        0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // Setup depth framebuffer
    glGenFramebuffers(1, &m_DepthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Setup depth shader
    auto depthShader = std::make_shared<Shader>("shaders/simpleDepth.vert", "shaders/simpleDepth.frag");
    m_Shaders.insert(std::make_pair("Depth", depthShader));

    auto blinnPhongShader = std::make_shared<Shader>("shaders/blinnPhong.vert", "shaders/blinnPhong.frag");
    blinnPhongShader->use();
    blinnPhongShader->setInt("texture1", 0);
    blinnPhongShader->setInt("shadowMap", 1);
    blinnPhongShader->setVec3("lightPos", m_LightPos);
    blinnPhongShader->setVec3("cameraPos", camera->getPos());
    blinnPhongShader->setBool("blinn", GL_TRUE);
    blinnPhongShader->setInt("blinnPower", 32);
    blinnPhongShader->setInt("phongPower", 8);
    blinnPhongShader->setFloat("ambientStrength", 0.05f);
    blinnPhongShader->setFloat("gamma", 2.2f);
    m_Shaders.insert(std::make_pair("BlinnPhong", blinnPhongShader));

    auto plane = std::make_shared<Plane>(blinnPhongShader, glm::vec3(0.0f, -0.5f, 0.0f),
        "textures/wood.png", 25.f, 25.f);
    m_Renderables.push_back(plane);

    auto cube = std::make_shared<Cube>(blinnPhongShader, glm::vec3(0.0f, 1.5f, 0.0), "textures/brickwall.jpg");
    cube->scale(glm::vec3(0.5));
    m_Renderables.push_back(cube);
    cube = std::make_shared<Cube>(blinnPhongShader, glm::vec3(2.0f, 0.0f, 2.0), "textures/brickwall.jpg");
    cube->scale(glm::vec3(0.5));
    m_Renderables.push_back(cube);
    cube = std::make_shared<Cube>(blinnPhongShader, glm::vec3(-2.0f, 0.0f, -2.0), "textures/brickwall.jpg");
    cube->scale(glm::vec3(0.5));
    m_Renderables.push_back(cube);
    cube = std::make_shared<Cube>(blinnPhongShader, glm::vec3(1.0f, 0.8f, -1.0), "textures/brickwall.jpg");
    cube->rotate(glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    cube->scale(glm::vec3(0.6));
    m_Renderables.push_back(cube);
    cube = std::make_shared<Cube>(blinnPhongShader, glm::vec3(-1.0f, 1.2f, 1.5), "textures/brickwall.jpg");
    cube->scale(glm::vec3(0.5));
    cube->rotate(glm::radians(30.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    m_Renderables.push_back(cube);
    cube = std::make_shared<Cube>(blinnPhongShader, glm::vec3(-1.0f, 0.0f, 2.0), "textures/brickwall.jpg");
    cube->rotate(glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    cube->scale(glm::vec3(0.25));
    m_Renderables.push_back(cube);
}

AdvancedLightningScene::~AdvancedLightningScene()
{
    glDeleteFramebuffers(1, &m_DepthMapFBO);
    glDeleteTextures(1, &m_DepthMap);
}

void AdvancedLightningScene::activate()
{
    Scene::activate();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
}

void AdvancedLightningScene::deactivate()
{
    Scene::deactivate();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
}

void AdvancedLightningScene::render(glm::mat4 & projection, glm::mat4 & view)
{
    // STEP 1: Render depth to the framebuffer (depth map)
    glm::mat4 lightSpaceMatrix = m_DirLight.lightProjection * m_DirLight.lightView;
    auto depthShader = m_Shaders["Depth"];
    depthShader->use();
    depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    glViewport(0, 0, SHADOW_RES_W, SHADOW_RES_H);
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    for (auto it = m_Renderables.begin(); it != m_Renderables.end(); it++)
    {
        glm::mat4 model = (*it)->getModelMatrix();
        depthShader->setMat4("model", model);
        (*it)->render();
    }
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Step 2
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_DepthMap);
    for (auto it = m_Renderables.begin(); it != m_Renderables.end(); it++)
    {
        auto currentShader = (*it)->getShader();
        currentShader->use();
        currentShader->setMat4("view", view);
        currentShader->setMat4("projection", projection);
        glm::mat4 model = (*it)->getModelMatrix();
        currentShader->setMat4("model", model);
        currentShader->setVec3("cameraPos", m_Camera->getPos());
        currentShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        (*it)->render();
    }
}

void AdvancedLightningScene::renderGUI()
{
    // Object list setup
    float width = 300, height = 200;

    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_Once);
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - width - GUI::MARGIN, GUI::MARGIN), ImGuiSetCond_Once);

    ImGui::Begin("Light configuration");
    auto lightShader = m_Shaders["BlinnPhong"];
    auto uniforms = lightShader->getUserSettableUniforms();
    GUI::renderShaderOptions(lightShader.get(), uniforms);

    GLfloat values[3] = { m_LightPos.x, m_LightPos.y, m_LightPos.z };
    if (ImGui::InputFloat3("Light Position", &values[0]))
    {
        m_LightPos = glm::vec3(values[0], values[1], values[2]);
        m_DirLight.lightView = glm::lookAt(m_LightPos,
            glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    ImGui::End();
}
