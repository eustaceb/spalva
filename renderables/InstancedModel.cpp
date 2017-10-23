#include "InstancedModel.h"

#include "../resourcing/ResourceManager.h"
#include "../resourcing/Texture.h"

InstancedModel::InstancedModel(std::shared_ptr<Shader> shader, const glm::vec3 & pos, 
    const GLchar * path, const unsigned int &count, const std::vector<glm::mat4> &modelMatrices, const std::string & label)
    : Model(shader, pos, path, label), m_Count(count)
{
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
    for (unsigned int i = 0; i < m_Meshes.size(); i++) 
    {
        GLuint specularCount = 1, diffuseCount = 1;
        auto textures = m_Meshes[i]->m_Textures;
        for (GLuint j = 0; j < textures.size(); j++)
        {
            Texture* tex = ResourceManager::instance()->getResource<Texture>(textures[j]);
            glActiveTexture(GL_TEXTURE0 + i);
            std::string texType = tex->getType();
            if (texType == "texture_diffuse")
                m_Shader->setInt(
                    ("material." + texType + std::to_string(diffuseCount++)).c_str(), i);
            else if (texType == "texture_specular")
                m_Shader->setInt(
                    ("material." + texType + std::to_string(specularCount++)).c_str(), i);
            tex->bind();
        }
        glActiveTexture(GL_TEXTURE0);

        m_Shader->setFloat("material.shininess", 16.0f);
        // Draw
        glBindVertexArray(m_Meshes[i]->getVAO());
        glDrawElementsInstanced(GL_TRIANGLES, m_Meshes[i]->m_Indices.size(),
            GL_UNSIGNED_INT, 0, m_Count);
        glBindVertexArray(0);
        // Cleanup
        for (GLuint i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            Texture::unbind();
        }
    }
}
