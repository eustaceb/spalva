//
// Created by eustace on 24/05/17.
//

#include "Mesh.h"
#include "../Shader.h"
#include "../resourcing/Texture.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, 
    std::vector<std::shared_ptr<Texture>> textures)
    : m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
{
    this->setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), &m_Indices[0], GL_STATIC_DRAW);

    // Pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    // Texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::render(std::shared_ptr<Shader> shader)
{
    // @TODO: Abstract material dependence
    // Prepare textures
    GLuint specularCount = 1, diffuseCount = 1;
    for (GLuint i = 0; i < m_Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string texType = m_Textures[i]->getType();
        if (texType == "texture_diffuse")
            glUniform1i(shader->getUniform(("material." + texType + std::to_string(diffuseCount++)).c_str()), i);
        else if (texType == "texture_specular")
            glUniform1i(shader->getUniform(("material." + texType + std::to_string(specularCount++)).c_str()), i);
        m_Textures[i]->bind();
    }
    glActiveTexture(GL_TEXTURE0);

    glUniform1f(shader->getUniform("material.shininess"), 16.0f);

    // Draw
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // Cleanup
    for (GLuint i = 0; i < m_Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        m_Textures[i]->unbind();
    }
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_VAO);
}

GLuint Mesh::getVAO() const
{
    return m_VAO;
}
