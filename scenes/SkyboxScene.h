#pragma once
#ifndef _SkyboxScene_H_
#define _SkyboxScene_H_

#include "Scene.h"

#include <memory>

#include "../renderables/Skybox.h"

class SkyboxScene :
    public Scene
{
public:
    SkyboxScene(std::shared_ptr<Camera> camera);
    ~SkyboxScene();

    void activate();
    void deactivate();
    void render(glm::mat4 &projection, glm::mat4 &view);
    void renderGUI();

private:
    std::unique_ptr<Skybox> m_Skybox;
    std::vector<std::string> m_ShaderLabels;
};

#endif // _SkyboxScene_H_
