#include "Scene.h"

Scene::Scene(std::shared_ptr<Camera> camera)
    : m_Camera(camera), m_CameraState(camera->getState())
{
}

Scene::~Scene()
{
}

void Scene::activate()
{
    m_Camera->setState(m_CameraState);
}

void Scene::deactivate()
{
    m_CameraState = m_Camera->getState();
}
