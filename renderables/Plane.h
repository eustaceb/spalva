#pragma once
#include "Renderable.h"
#ifndef _Plane_H_
#define _Plane_H_

class Shader;

class Plane :
    public Renderable
{
public:
    Plane(std::shared_ptr<Shader> shader, const glm::vec3 &pos, const std::string &texturePath, 
        const float &width = 1.0f, const float &length = 1.0f, const std::string &label = "Plane");
    ~Plane();

    void render();
private:
    GLuint m_Texture;
    GLuint m_VBO, m_VAO;
};

#endif // _Plane_H_