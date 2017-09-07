#pragma once
#ifndef _LightModel_H_
#define _LightModel_H_


#include <glad/glad.h>
#include <vector>
#include <string>
#include <map>
#include <memory>

#include "Model.h"

class Shader;
class Camera;
class LightCube;

class LightModel : public Model {
public:
    LightModel(std::shared_ptr<Shader> shader, const glm::vec3 &pos, 
    const GLchar *path, std::shared_ptr<Camera> camera, const std::string &label = "LightModel");

    void render();

    void lightUpdated(GLuint index);
    void addPointLight(std::weak_ptr<LightCube> light, GLuint index);
    void setCamera(std::shared_ptr<Camera> camera);
private:
    std::map<GLuint, std::weak_ptr<LightCube>> m_PointLights;
    std::shared_ptr<Camera> m_Camera;
};

#endif //_LightModel_H_
