#pragma once
#ifndef _Skybox_H_
#define _Skybox_H_

#include "Renderable.h"

#include <vector>
#include <glm/glm.hpp>

class Skybox :
    public Renderable
{
public:
    Skybox();
    Skybox(std::shared_ptr<Shader> shader, const glm::vec3 &pos, 
        const std::vector<std::string> &faces, const std::string &label = "Skybox");

    ~Skybox();

    void render();

private:
    GLuint m_Texture;
    GLuint m_VBO, m_VAO;

    GLuint loadCubemap(std::vector<std::string> faces);
};

#endif //_Skybox_H_