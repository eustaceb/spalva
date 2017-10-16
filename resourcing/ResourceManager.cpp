#define NOMINMAX
#include <iostream>
#include <limits>

#include "../Definitions.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "../Helpers.h"

ResourceManager* ResourceManager::s_Instance = 0;
unsigned int ResourceManager::s_Timestamp = 0;

void ResourceGroup::load()
{
    for (auto res : resources)
    {
        bool& loaded = res.second;
        if (!loaded)
        {
            res.first->load();
            loaded = true;
        }
    }
    loadedMemory = memory;
}

void ResourceGroup::unload()
{
    for (auto res : resources)
    {
        bool& loaded = res.second;
        if (loaded)
        {
            res.first->unload();
            loaded = false;
        }
    }
    loadedMemory = 0;
}

ResourceManager::ResourceManager()
    : m_MaxLoad(RM_MAX_LOAD_BYTES)
{
    createGroup("default");
    m_ActiveGroup = &m_Groups.find("default")->second;
    std::cout << "Initialized the ResourceManager with ";
    std::cout << Helpers::bytesToMB(RM_MAX_LOAD_BYTES) <<"MB cap." << std::endl;
}

ResourceManager::~ResourceManager()
{
    for (auto it = m_Groups.begin(); it != m_Groups.end(); it++)
    {
        std::string groupName = (*it).first;
        removeGroup(groupName);
    }
}

ResourceManager * ResourceManager::instance()
{
    if (s_Instance == 0)
        s_Instance = new ResourceManager;
    return s_Instance;
}
// @TODO: Consider renaming to bindGroup
void ResourceManager::accessGroup(const std::string & groupName)
{
    auto group = m_Groups.find(groupName);
    assert(group != m_Groups.end());
    m_ActiveGroup = &group->second;
    if (RM_CACHING_ENABLED != 0)
    {
        m_ActiveGroup->timestamp = s_Timestamp++;
    }
}

void ResourceManager::createGroup(const std::string & groupName)
{
    m_Groups.insert(std::make_pair(groupName, ResourceGroup(s_Timestamp++)));
}

void ResourceManager::removeGroup(const std::string & groupName)
{
    auto it = m_Groups.find(groupName);
    if (it != m_Groups.end())
    {
        it->second.unload();
        m_Groups.erase(it);
    }
}

void ResourceManager::loadGroup(const std::string & groupName)
{
    auto it = m_Groups.find(groupName);
    if (it != m_Groups.end())
    {
        ResourceGroup& group = it->second;
        unsigned long long toLoad = group.memory - group.loadedMemory;
        if (toLoad == 0) return; // Nothing to load
        if (RM_CACHING_ENABLED != 0)
        {
            std::cout << "Loading group \"" << groupName << "\" (";
            std::cout << Helpers::bytesToMB(toLoad) << " MB)\n";
            while (m_CurrentLoad + toLoad > m_MaxLoad)
            {
                std::cout << "Running LRU optimization (";
                std::cout << Helpers::bytesToMB(m_CurrentLoad) << "/";
                std::cout << Helpers::bytesToMB(m_MaxLoad) << " MB taken)\n";

                // If can't reduce any further - stop trying
                if (!reduceLoad()) break;
            }
        }
        group.load();
        m_CurrentLoad += toLoad;
        std::cout << "Current load " << Helpers::bytesToMB(m_CurrentLoad);
        std::cout << " MB. " << std::endl;
    }
}

void ResourceManager::unloadGroup(const std::string & groupName)
{
    auto it = m_Groups.find(groupName);
    if (it != m_Groups.end())
    {
        it->second.unload();
    }
}

/**
    Defaults to current active texture
**/
void ResourceManager::addTexture(std::shared_ptr<Texture> tex, 
    const std::string & groupName)
{
    assert(groupName != "" || m_ActiveGroup != 0);
    // If tex not yet loaded
    if (m_Textures.find(groupName) == m_Textures.end())
        m_Textures.insert(std::make_pair(tex->getPath(), tex));

    if (groupName != "")
    {
        auto group = m_Groups.find(groupName);
        assert(group != m_Groups.end());
        group->second.resources.push_back(std::make_pair(tex, false));
        group->second.memory += tex->getMemoryTaken();
    }
    else
    {
        m_ActiveGroup->resources.push_back(std::make_pair(tex, false));
        m_ActiveGroup->memory += tex->getMemoryTaken();
    }
}

/**
    Returns 
        true if load was reduced
        false if can't reduce any further.
**/
bool ResourceManager::reduceLoad()
{
    unsigned int minTimestamp = std::numeric_limits<unsigned int>::max();
    std::string lru = "";
    // A more optimal solution could be written 
    // but O(n) each time should suffice for now
    for (auto it = m_Groups.begin(); it != m_Groups.end(); it++)
    {
        ResourceGroup& group = it->second;
        if (group.timestamp < minTimestamp && group.loadedMemory != 0 
            && std::addressof(group) != std::addressof(*m_ActiveGroup))
        {
            minTimestamp = group.timestamp;
            lru = it->first;
        }
    }
    if (lru != "")
    {
        ResourceGroup& group = m_Groups.find(lru)->second;
        m_CurrentLoad -= group.loadedMemory;
        group.unload();
        std::cout << "Unloaded group \"" << lru << "\"" << std::endl;
        return true;
    }
    std::cout << "Can't optimize any further" << std::endl;
    return false;
}
