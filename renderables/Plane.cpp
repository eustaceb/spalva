#include "Plane.h"

#include <stb/stb_image.h>
#include <iostream>

#include "../Helpers.h"
/*#include <glad/glad.h>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>*/

//#include "../Shader.h"

Plane::Plane(std::shared_ptr<Shader> shader, const glm::vec3 & pos, const std::string &texturePath, 
    const float &width, const float &length, const std::string & label)
    : Renderable(shader, pos, label)
{
    float planeVertices[] = {
        // positions            // normals         // texcoords
        width, 0.0f,  length,  0.0f, 1.0f, 0.0f,  width,  0.0f,
        -width, 0.0f,  length,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -width, 0.0f, -length,  0.0f, 1.0f, 0.0f,   0.0f, length,

        width, 0.0f,  length,  0.0f, 1.0f, 0.0f,  width,  0.0f,
        -width, 0.0f, -length,  0.0f, 1.0f, 0.0f,   0.0f, length,
        width, 0.0f, -length,  0.0f, 1.0f, 0.0f,  width, length
    };
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);

    // Pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);

    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    // TexCoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

    glBindVertexArray(0);

    m_Texture = Helpers::loadTexture(texturePath);
}

Plane::~Plane()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteTextures(1, &m_Texture);
}

void Plane::render()
{
    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}