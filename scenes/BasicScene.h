#pragma once
#ifndef _BasicScene_H_
#define _BasicScene_H_

#include <memory>

#include "Scene.h"

class Camera;
class Skybox;

class BasicScene :
    public Scene
{
public:
    BasicScene(std::shared_ptr<Camera> camera);

    void activate();
    void deactivate();
    void render(glm::mat4 &projection, glm::mat4 &view);
    void renderGUI();
private:
    std::unique_ptr<Skybox> m_Skybox;
};

#endif // _BasicScene_H_