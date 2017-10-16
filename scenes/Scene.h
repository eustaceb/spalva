#pragma once
#ifndef _Scene_H_
#define _Scene_H_

#include <glm/common.hpp>
#include <vector>
#include <memory>
#include <map>

#include "../Camera.h"

class Renderable;
class Shader;

class Scene
{
public:
    Scene(std::shared_ptr<Camera> camera, const std::string & name);
    virtual ~Scene();

    virtual void activate(); // restores camera state, activates resource group
    virtual void deactivate(); // captures camera state

    virtual void update(GLfloat deltaTime);
    virtual void render(glm::mat4 & projection, glm::mat4 & view) = 0;
    virtual void renderGUI() = 0;

    std::string getName() const;
protected:
    CameraState m_CameraState;
    std::string m_Name;
    std::shared_ptr<Camera> m_Camera;
    std::vector <std::shared_ptr<Renderable>> m_Renderables;
    std::map<std::string, std::shared_ptr<Shader>> m_Shaders;
};
#endif // _Scene_H_