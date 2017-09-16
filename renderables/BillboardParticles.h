#pragma once
#ifndef _BillboardParticles_H_
#define _BillboardParticles_H_

#include <glad/glad.h>
#include <glm/common.hpp>
#include "Renderable.h"

#include <vector>

struct Particle {
    size_t id;
    glm::vec3 startingPos;
    glm::vec3 velocity;
    double lifetime;
    double defaultLifetime;
};

class Camera;

class BillboardParticles
{
public:
    BillboardParticles(std::shared_ptr<Shader> shader, const glm::vec3 &pos, 
        const float & size, const size_t & count, const float & speed = 0.05,
        const glm::vec3 & color = glm::vec3(1.0, 0.2, 0.0),
        const glm::vec3 & dirMin = glm::vec3(-0.01, 0, -0.01),
        const glm::vec3 & dirMax = glm::vec3(0.01, 0.04, 0.01), 
        const glm::vec2 & lifetime = glm::vec2(1.0, 7.0), const int & randomness = 200);
    ~BillboardParticles();

    void render(glm::mat4 view);
    void update(const double &deltaTime);

    void resetParticle(Particle & p);

    std::shared_ptr<Shader> getShader() const;

private:
    void generateParticles(const size_t & count, const float & speed, const glm::vec3 & color, 
        const glm::vec3 & dirMin, const glm::vec3 & dirMax, const glm::vec2 & lifetime,
        const int & randomness);

    size_t m_Count;
    glm::vec3 m_Color;

    GLuint m_VAO, m_VBO, m_ColorAB, m_PositionsAB;
    glm::vec3 m_Pos;
    std::shared_ptr<Shader> m_Shader;
    std::vector<glm::vec3> m_Positions;
    std::vector<glm::vec4> m_ColorVectors;
    std::vector<Particle> m_Particles;
};

#endif // _BillboardParticles_H_