//
// Created by eustace on 18/05/17.
//

#include <iostream>
#include "Renderable.h"
#include "../resourcing/Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderable::Renderable(std::shared_ptr<Shader> shader, const glm::vec3 &pos, const std::string &label)
    : m_Shader(shader), m_Pos(pos), m_Label(label)
{
    m_ModelMatrix = glm::translate(glm::mat4(), m_Pos);
}


std::shared_ptr<Shader> Renderable::getShader() const
{
    return m_Shader;
}

glm::mat4 Renderable::getModelMatrix() const
{
    return m_ModelMatrix;
}

void Renderable::setLabel(const std::string & label)
{
    m_Label = label;
}

std::string Renderable::getLabel() const
{
    return m_Label;
}

void Renderable::setPos(glm::vec3 pos)
{
    if (pos == m_Pos) return;
    m_ModelMatrix = glm::translate(m_ModelMatrix, m_Pos - pos);
    m_Pos = pos;
}

void Renderable::rotate(GLfloat angle, glm::vec3 axis)
{
    m_ModelMatrix = glm::rotate(m_ModelMatrix, angle, axis);
}

void Renderable::scale(glm::vec3 axis)
{
    m_ModelMatrix = glm::scale(m_ModelMatrix, axis);
}

glm::vec3 Renderable::getPos() const
{
    return m_Pos;
}

void Renderable::setShader(std::shared_ptr<Shader> shader)
{
    m_Shader = shader;
}

void Renderable::setModelMatrix(glm::mat4 model)
{
    m_ModelMatrix = model;
}

