#pragma once
#include "Scene.h"

struct DirectionalLight
{
    float nearPlane;
    float farPlane;
    glm::mat4 lightProjection;
    glm::mat4 lightView;
};
class AdvancedLightningScene :
    public Scene
{
public:
    AdvancedLightningScene(std::shared_ptr<Camera> camera);
    ~AdvancedLightningScene();

    void activate();
    void deactivate();
    void render(glm::mat4 &projection, glm::mat4 &view);
    void renderGUI();
private:
    DirectionalLight m_DirLight;
    const GLsizei SHADOW_RES_W = 1024, SHADOW_RES_H = 1024;
    GLuint m_DepthMapFBO, m_DepthMap;
    GLuint m_QuadVAO, m_QuadVBO;
    glm::vec3 m_LightPos;
};