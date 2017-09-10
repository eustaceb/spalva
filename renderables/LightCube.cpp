//
// Created by eustace on 19/05/17.
//

#include <vector>
#include "LightCube.h"
#include <glm/gtc/matrix_transform.hpp>

LightCube::LightCube(std::shared_ptr<Shader> shader, const glm::vec3 &pos, const GLuint &index,
    const std::string &label)
: Renderable(shader, pos, label), m_Index(index)
{
    GLfloat vertices[] = {
            // Positions           // Normals           // Texture Coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    //glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

//    m_Indices = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), &m_Indices[0], GL_STATIC_DRAW);

    // Pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)3);

    glBindVertexArray(0);
}

void LightCube::render()
{
    glUniform3f(m_Shader->getUniform("lightColor"), m_Color.r, m_Color.g, m_Color.b);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

glm::vec3 LightCube::getAmbient() const
{
    return m_Ambient;
}
void LightCube::setAmbient(glm::vec3 ambient)
{
    m_Ambient = ambient;
    mixColors();
}

glm::vec3 LightCube::getDiffuse() const
{
    return m_Diffuse;
}

glm::vec3 LightCube::getSpecular() const
{
    return m_Specular;
}

GLfloat LightCube::getConstant() const
{
    return m_Constant;
}
void LightCube::setConstant(GLfloat constant)
{
    m_Constant = constant;
    notifyAll();
}

GLfloat LightCube::getLinear() const
{
    return m_Linear;
}
void LightCube::setLinear(GLfloat linear)
{
    m_Linear = linear;
    notifyAll();
}
GLfloat LightCube::getQuadratic() const
{
    return m_Quadratic;
}

void LightCube::setQuadratic(GLfloat quadratic)
{
    m_Quadratic = quadratic;
    notifyAll();
}

void LightCube::setDiffuse(glm::vec3 diffuse)
{
    m_Diffuse = diffuse;
    mixColors();
}

void LightCube::setSpecular(glm::vec3 specular)
{
    m_Specular = specular;
    mixColors();
}

void LightCube::mixColors()
{
    m_Color = m_Diffuse;
    notifyAll();
}

LightCube::~LightCube()
{
    glDeleteBuffers(1, &m_VBO);
    //glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_VAO);
}
// @TODO: Abstract LightModel once necessary
void LightCube::addObserver(std::weak_ptr<LightModel> m)
{
    m_Observers.push_back(m);
}

void LightCube::notifyAll()
{
    for (auto m : m_Observers)
        if (auto obs = m.lock())
            obs->lightUpdated(m_Index);
}
