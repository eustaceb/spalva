#pragma once
#ifndef _ParticlesScene_H_
#define _ParticlesScene_H_

#include "Scene.h"

class BillboardParticles;

class ParticlesScene :
    public Scene
{
public:
    ParticlesScene(std::shared_ptr<Camera> camera);
    ~ParticlesScene();

    void activate();
    void deactivate();

    void update(GLfloat deltaTime);
    void render(glm::mat4 &projection, glm::mat4 &view);
    void renderGUI();
private:
    std::unique_ptr<BillboardParticles> m_ParticleEmitter;
};

#endif // _ParticlesScene_H_

