
//
// Created by eustace on 19/05/17.
//

#ifndef _Cube_H_
#define _Cube_H_


#include "Renderable.h"

class Shader;
class Texture;

class Cube : public Renderable {
public:
    Cube(std::shared_ptr<Shader> shader, const glm::vec3 &pos, const std::string &texturePath = "",
        const float edgeSize = 1.0f, const std::string &label = "Cube");
    ~Cube();

    void render();

private:
    unsigned int m_Texture;
    GLuint m_VBO, m_VAO;
};


#endif //_Cube_H_
