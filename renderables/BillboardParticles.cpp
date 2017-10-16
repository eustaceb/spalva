#include "BillboardParticles.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../resourcing/Shader.h"

BillboardParticles::BillboardParticles(std::shared_ptr<Shader> shader, const glm::vec3 &pos, 
    const float & size, const size_t & count, const float & speed, const glm::vec3 & color, 
    const glm::vec3 & dirMin, const glm::vec3 & dirMax, const glm::vec2 & lifetime, 
    const int & randomness)
    : m_Shader(shader), m_Pos(pos), m_Count(count)
{
    generateParticles(count, speed, color, dirMin, dirMax, lifetime, randomness);
    // @TODO: In future, fix size to 0.5 and create an AB for dynamic particle sizing
    float quadVertices[] = {
        -size, size,
        size, size,
        size, -size,
        -size, size,
        size, -size,
        -size, -size,
    };
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO); // Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    // Vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    // Color
    glGenBuffers(1, &m_ColorAB); // Color data
    glBindBuffer(GL_ARRAY_BUFFER, m_ColorAB);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec4), &m_ColorVectors[0], GL_STREAM_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0);
    glVertexAttribDivisor(1, 1);
    // Position
    glGenBuffers(1, &m_PositionsAB);
    glBindBuffer(GL_ARRAY_BUFFER, m_PositionsAB);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), &m_Positions[0], GL_STREAM_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(2, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

BillboardParticles::~BillboardParticles()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_ColorAB);
    glDeleteBuffers(1, &m_PositionsAB);
    glDeleteVertexArrays(1, &m_VAO);
}

void BillboardParticles::render(glm::mat4 view)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glBindVertexArray(m_VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_Count);
    glBindVertexArray(0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void BillboardParticles::update(const double &deltaTime)
{
    // @TODO: Could be beneficial to have expiries on timestamp
    // This way particles could be sorted and updated more optimally
    // E.g. currentTime - timestamp > lifetime ? ALIVE : DEAD
    for (size_t i = 0; i < m_Count; i++)
    {
        m_Particles[i].lifetime -= deltaTime;
        if (m_Particles[i].lifetime <= 0)
        {
            resetParticle(m_Particles[i]);
        }
        else
        {
            m_ColorVectors[i].a = m_Particles[i].lifetime / m_Particles[i].defaultLifetime;
            m_Positions[i] += m_Particles[i].velocity;
        }
    }
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_ColorAB);
    glBufferData(GL_ARRAY_BUFFER, m_Count * sizeof(glm::vec4), &m_ColorVectors[0], GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_PositionsAB);
    glBufferData(GL_ARRAY_BUFFER, m_Count * sizeof(glm::vec3), &m_Positions[0], GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void BillboardParticles::resetParticle(Particle & p)
{
    // Reset position
    m_Positions[p.id] = p.startingPos;
    // Reset lifetime
    p.lifetime = p.defaultLifetime;
    // Reset color
    m_ColorVectors[p.id].a = 1.0f;
}

std::shared_ptr<Shader> BillboardParticles::getShader() const
{
    return m_Shader;
}

void BillboardParticles::generateParticles(const size_t & count, const float & speed, 
    const glm::vec3 & color, const glm::vec3 & dirMin, const glm::vec3 & dirMax, 
    const glm::vec2 & lifetime, const int & randomness)
{
    // @TODO: Consider std::array
    // Reserve memory for faster allocation when generating a lot of particles
    m_Particles.reserve(count);
    m_Positions.reserve(count);
    m_ColorVectors.reserve(count);


    // The formula: velMin + R * (|velMin - velMax|) / randomness
    // where R = rand() % randomness
    // (|velMin| + |velMax|)/randomness stands for density, i.e. how many vals to squeeze into interval
    float densityX = glm::abs(dirMax.x - dirMin.x) / randomness;
    float densityY = glm::abs(dirMax.y - dirMin.y) / randomness;
    dirMax.y < 0 ? densityY *= -1 : densityY *= 1;
    float densityZ = glm::abs(dirMax.z - dirMin.z) / randomness;
    // Lifetime density
    float densityL = (glm::abs(lifetime[0]) + glm::abs(lifetime[0])) / randomness;

    srand(glfwGetTime());
    for (size_t i = 0; i < count; i++)
    {
        Particle p;
        p.id = i;
        p.startingPos = m_Pos; // Starting position equal to spawner pos

        float dirX = dirMin.x + (rand() % randomness) * densityX;
        float dirY = dirMin.y + (rand() % randomness) * densityY;
        float dirZ = dirMin.z + (rand() % randomness) * densityZ;
        glm::vec3 normalizedDirs = glm::normalize(glm::vec3(dirX, dirY, dirZ));
        p.velocity = normalizedDirs * speed;

        p.defaultLifetime = lifetime[0] + (rand() % randomness) * densityL;
        p.lifetime = p.defaultLifetime;
        // Store starting pos
        m_Positions.push_back(p.startingPos);
        // Store the color
        m_ColorVectors.push_back(glm::vec4(color, 1.0f));

        m_Particles.push_back(p);
    }
}
