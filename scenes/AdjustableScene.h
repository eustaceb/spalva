#pragma once
#ifndef _AdjustableScene_H_
#define _AdjustableScene_H_

#include "Scene.h"

class AdjustableScene :
    public Scene
{
public:
    AdjustableScene(std::shared_ptr<Camera> camera, const std::string & name);
    ~AdjustableScene();

    void activate();
    void deactivate();
    void render(glm::mat4 &projection, glm::mat4 &view);
    void renderGUI();
};

#endif // _AdjustableScene_H_