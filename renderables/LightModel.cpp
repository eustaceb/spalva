//
// Created by eustace on 25/05/17.
//

#include <iostream>
#include <sstream>

#include "LightModel.h"
#include "LightCube.h"
#include "../Camera.h"

#include <glm/glm.hpp>


LightModel::LightModel(std::shared_ptr<Shader> shader, const glm::vec3 &pos, 
    const GLchar *path, std::shared_ptr<Camera> camera, const std::string &label)
    : Model(shader, pos, path, label), m_Camera(camera)
{
    if (std::strcmp(path, "") != 0)
        this->loadModel(path);

    // Setup lights
    glm::vec3 lightAmbient(0.05f), lightDiffuse(0.5f), lightSpecular(0.2f);

    // Directional light
    glUniform3f(m_Shader->getUniform("dirLight.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(m_Shader->getUniform("dirLight.ambient"), lightAmbient.r, lightAmbient.g, lightAmbient.b);
    glUniform3f(m_Shader->getUniform("dirLight.diffuse"), lightDiffuse.r, lightDiffuse.g, lightDiffuse.b);
    glUniform3f(m_Shader->getUniform("dirLight.specular"), lightSpecular.r, lightSpecular.g, lightSpecular.b);

    // Flashlight
    glUniform3f(m_Shader->getUniform("spotLight.position"), m_Camera->position.x, m_Camera->position.y, m_Camera->position.z);
    glUniform3f(m_Shader->getUniform("spotLight.direction"), m_Camera->front.x, m_Camera->front.y, m_Camera->front.z);
    glUniform1f(m_Shader->getUniform("spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
    glUniform1f(m_Shader->getUniform("spotLight.outerCutOff"), glm::cos(glm::radians(17.5f)));
    glUniform3f(m_Shader->getUniform("spotLight.ambient"), lightAmbient.r, lightAmbient.g, lightAmbient.b);
    glUniform3f(m_Shader->getUniform("spotLight.diffuse"), lightDiffuse.r, lightDiffuse.g, lightDiffuse.b);
    glUniform3f(m_Shader->getUniform("spotLight.specular"), lightSpecular.r, lightSpecular.g, lightSpecular.b);
    glUniform1f(m_Shader->getUniform("spotLight.constant"), 1.0f);
    glUniform1f(m_Shader->getUniform("spotLight.linear"), 0.09f);
    glUniform1f(m_Shader->getUniform("spotLight.quadratic"), 0.032f);
}

void LightModel::render()
{
    glUniform3f(m_Shader->getUniform("viewPos"), m_Camera->position.x, m_Camera->position.y, m_Camera->position.z);
    glUniform3f(m_Shader->getUniform("spotLight.position"), m_Camera->position.x, m_Camera->position.y, m_Camera->position.z);
    glUniform3f(m_Shader->getUniform("spotLight.direction"), m_Camera->front.x, m_Camera->front.y, m_Camera->front.z);

    Model::render();
}

void LightModel::setCamera(std::shared_ptr<Camera> camera)
{
    m_Camera = camera;
}

void LightModel::addPointLight(std::weak_ptr<LightCube> light, GLuint index)
{
    m_PointLights[index] = light;
    lightUpdated(index);
}

void LightModel::lightUpdated(GLuint index)
{
    auto l = m_PointLights.at(index);
    if (auto light = l.lock())
    {
        m_Shader->use();
        glm::vec3 pos = light->getPos(), ambient = light->getAmbient(),
            diffuse = light->getDiffuse(), specular = light->getSpecular();
        std::ostringstream lightName;
        lightName << "pointLights[" << index << "].";
        // TODO: Validate this, might cause mem issues
        glUniform3f(m_Shader->getUniform((lightName.str() + "position").c_str()), pos.x, pos.y, pos.z);
        glUniform3f(m_Shader->getUniform((lightName.str() + "ambient").c_str()), ambient.r, ambient.g, ambient.b);
        glUniform3f(m_Shader->getUniform((lightName.str() + "diffuse").c_str()), diffuse.r, diffuse.g, diffuse.b);
        glUniform3f(m_Shader->getUniform((lightName.str() + "specular").c_str()), specular.r, specular.g, specular.b);
        glUniform1f(m_Shader->getUniform((lightName.str() + "constant").c_str()), light->getConstant());
        glUniform1f(m_Shader->getUniform((lightName.str() + "linear").c_str()), light->getLinear());
        glUniform1f(m_Shader->getUniform((lightName.str() + "quadratic").c_str()), light->getQuadratic());
    }
}