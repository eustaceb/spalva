//
// Created by eustace on 19/05/17.
//

#include "Cube.h"
#include "../Helpers.h"


Cube::Cube(std::shared_ptr<Shader> shader, const glm::vec3 &pos, const std::string &texturePath,
    const float edgeSize, const std::string &label)
        : Renderable(shader, pos, label), m_Texture(0)
{
    // @TODO: Should tex coords be in [0, 1]?
    const float halfEdge = edgeSize / 2;
    GLfloat vertices[] = {
        // Positions           // Normals           // Texture Coords
        -halfEdge, -halfEdge, -halfEdge,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        halfEdge, -halfEdge, -halfEdge,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        halfEdge,  halfEdge, -halfEdge,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        halfEdge,  halfEdge, -halfEdge,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -halfEdge,  halfEdge, -halfEdge,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -halfEdge, -halfEdge, -halfEdge,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -halfEdge, -halfEdge,  halfEdge,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        halfEdge, -halfEdge,  halfEdge,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        halfEdge,  halfEdge,  halfEdge,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        halfEdge,  halfEdge,  halfEdge,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -halfEdge,  halfEdge,  halfEdge,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -halfEdge, -halfEdge,  halfEdge,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -halfEdge,  halfEdge,  halfEdge, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -halfEdge,  halfEdge, -halfEdge, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -halfEdge, -halfEdge, -halfEdge, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -halfEdge, -halfEdge, -halfEdge, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -halfEdge, -halfEdge,  halfEdge, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -halfEdge,  halfEdge,  halfEdge, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        halfEdge,  halfEdge,  halfEdge,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        halfEdge,  halfEdge, -halfEdge,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        halfEdge, -halfEdge, -halfEdge,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        halfEdge, -halfEdge, -halfEdge,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        halfEdge, -halfEdge,  halfEdge,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        halfEdge,  halfEdge,  halfEdge,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -halfEdge, -halfEdge, -halfEdge,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        halfEdge, -halfEdge, -halfEdge,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        halfEdge, -halfEdge,  halfEdge,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        halfEdge, -halfEdge,  halfEdge,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -halfEdge, -halfEdge,  halfEdge,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -halfEdge, -halfEdge, -halfEdge,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -halfEdge,  halfEdge, -halfEdge,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        halfEdge,  halfEdge, -halfEdge,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        halfEdge,  halfEdge,  halfEdge,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        halfEdge,  halfEdge,  halfEdge,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -halfEdge,  halfEdge,  halfEdge,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -halfEdge,  halfEdge, -halfEdge,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    // Pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    if (texturePath != "")
    {
        // Texture
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        m_Texture = Helpers::loadTexture(texturePath);
    }
    glBindVertexArray(0);
}

Cube::~Cube()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
    if (m_Texture != 0)
        glDeleteTextures(1, &m_Texture);
}


void Cube::render()
{
    glBindVertexArray(m_VAO);
    if (m_Texture != 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Texture);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}