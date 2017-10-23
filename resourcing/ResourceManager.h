#pragma once
#ifndef _ResourceManager_H_
#define _ResourceManager_H_
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <set>

class Resource;
class Texture;
class Shader;

struct ResourceGroup {
    ResourceGroup(const unsigned int & timestamp) : timestamp(timestamp), 
        memory(0), loadedMemory(0) {};
    // <Resource, isLoaded>
    std::vector<std::pair<unsigned int, bool>> resources;

    unsigned long long memory;
    unsigned long long loadedMemory;
    unsigned int timestamp;
};

// @TODO! Memory taken should be recalculated due to multiple
// groups potentially using the same textures
class ResourceManager
{
public:
    static ResourceManager* instance();
    void bindGroup(const std::string & groupName);
    void accessGroup(const std::string & groupName);
    void createGroup(const std::string & groupName);
    void removeGroup(const std::string & groupName);
    void loadGroup(const std::string & groupName);
    void unloadGroup(const std::string & groupName);
    void reloadActiveGroup();
    //void mergeGroups(const std::string & group1, const std::string & group1, const std::string & newName);
    void loadGroupResources(ResourceGroup& group);
    void unloadGroupResources(ResourceGroup& group);

    template <class T, typename ...Targs> 
    unsigned int createResource(Targs&&... args)
    {
        assert(m_ActiveGroup != 0);
        unsigned int resultID = 0;
        auto resource = std::make_unique<T>(args...);
        auto lookup = m_Registry.find(resource->getPath());
        if (lookup != m_Registry.end())
        {
            resultID = lookup->second;
        }
        if (resultID == 0)
        {
            m_Resources.push_back(std::move(resource));
            resultID = ++s_ResourceIDCounter;
            m_Registry.insert(
                std::make_pair(m_Resources[resultID]->getPath(), resultID));
        }
        m_ActiveGroup->resources.push_back(std::make_pair(resultID, false));
        m_ActiveGroup->memory += m_Resources[resultID]->getMemoryTaken();
        return resultID;
    }

    template <class T>
    T* getResource(unsigned int id)
    {
        assert(id > 0 && id <= m_Resources.size());
        return dynamic_cast<T*>(m_Resources[id].get());
    }

private:
    static ResourceManager* s_Instance;
    ResourceManager();
    ~ResourceManager();

    bool reduceLoad();

    ResourceGroup* m_ActiveGroup;
    static unsigned int s_ResourceIDCounter;
    static unsigned int s_Timestamp;
    const unsigned long long m_MaxLoad;
    unsigned long long m_CurrentLoad;

    std::map<std::string, ResourceGroup> m_Groups;
    //std::set<unsigned int> m_FreeIDs;
    std::map<std::string, unsigned int> m_Registry;
    std::vector<std::unique_ptr<Resource>> m_Resources;
};

#endif // _ResourceManager_H_