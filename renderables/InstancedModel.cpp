#include "InstancedModel.h"


InstancedModel::InstancedModel(std::shared_ptr<Shader> shader, const glm::vec3 & pos, 
    const GLchar * path, const unsigned int &count, const std::vector<glm::mat4> &modelMatrices, const std::string & label)
    : Model(shader, pos, path, label), m_Count(count)
{
    unsigned int buffer;
    glGenBuffers(1, &m_InstanceMatrixAB);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceMatrixAB);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        unsigned int VAO = m_Meshes[i]->getVAO();
        glBindVertexArray(VAO);
        // vertex Attributes
        GLsizei vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}

InstancedModel::~InstancedModel()
{
    glDeleteBuffers(1, &m_InstanceMatrixAB);
}

void InstancedModel::render()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TexturesLoaded[0].id);

    for (int i = 0; i < m_Meshes.size(); i++) 
    {
        glBindVertexArray(m_Meshes[i]->getVAO());
        glDrawElementsInstanced(GL_TRIANGLES, m_Meshes[i]->m_Indices.size(), 
            GL_UNSIGNED_INT, 0, m_Count);
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
