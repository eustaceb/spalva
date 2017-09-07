#pragma once
#ifndef _InstancingScene_H_
#define _InstancingScene_H_

#include "Scene.h"

class Skybox;

class InstancingScene :
    public Scene
{
public:
    InstancingScene(std::shared_ptr<Camera> camera);
    ~InstancingScene();

    void activate();
    void deactivate();
    void render(glm::mat4 &projection, glm::mat4 &view);
    void renderGUI();

    void generateAsteroids(const unsigned int &count, const float &radius, const float &offset);
private:
    // place the models with an z offset so that we don't have to move the camera from origin
    const float Z_DISPLACEMENT = -16.0f;

    std::unique_ptr<Skybox> m_Skybox;
};

#endif // _InstancingScene_H_

