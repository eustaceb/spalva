#include "Scene.h"
#include "../resourcing/ResourceManager.h"

Scene::Scene(std::shared_ptr<Camera> camera, const std::string & name)
    : m_Camera(camera), m_CameraState(camera->getState()), m_Name(name)
{
    ResourceManager::instance()->createGroup(m_Name);
    ResourceManager::instance()->bindGroup(m_Name);
}

Scene::~Scene()
{
}

void Scene::activate()
{
    m_Camera->setState(m_CameraState);
    ResourceManager::instance()->accessGroup(m_Name);
    ResourceManager::instance()->loadGroup(m_Name);
}

void Scene::deactivate()
{
    m_CameraState = m_Camera->getState();
}

void Scene::update(GLfloat deltaTime)
{
    
}

std::string Scene::getName() const
{
    return m_Name;
}
