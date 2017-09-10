//
// Created by eustace on 18/05/17.
//
#pragma once
#ifndef _Renderable_H_
#define _Renderable_H_

#include <string>
#include <memory>
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>

class Shader;


class Renderable {
public:
    Renderable(std::shared_ptr<Shader> shader, const glm::vec3 &pos = glm::vec3(0.f), 
        const std::string &label = "No label");

    virtual void render() = 0;

    void setLabel(const std::string &label);
    std::string getLabel() const;

    void setPos(glm::vec3 pos);
    glm::vec3 getPos() const;

    void setShader(std::shared_ptr<Shader> shader);
    std::shared_ptr<Shader> getShader() const;

    void rotate(GLfloat angle, glm::vec3 axis);
    void scale(glm::vec3 axis);

    void setModelMatrix(glm::mat4 model);
    glm::mat4 getModelMatrix() const;
protected:
    std::string m_Label;
    std::shared_ptr<Shader> m_Shader;
    glm::vec3 m_Pos;
    glm::mat4 m_ModelMatrix;
};


#endif //_Renderable_H_
