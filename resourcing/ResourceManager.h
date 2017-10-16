#pragma once
#ifndef _ResourceManager_H_
#define _ResourceManager_H_
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <queue>

class Resource;
class Texture;

struct ResourceGroup {
    ResourceGroup(const unsigned int & timestamp) : timestamp(timestamp), 
        memory(0), loadedMemory(0) {};
    // <Resource, isLoaded>
    std::vector<std::pair<std::shared_ptr<Resource>, bool>> resources;

    unsigned long long memory;
    unsigned long long loadedMemory;
    unsigned int timestamp;

    void load();
    void unload();
};

// @TODO! Memory taken should be recalculated due to multiple
// groups potentially using the same textures
class ResourceManager
{
public:
    static ResourceManager* instance();
    void accessGroup(const std::string & groupName);
    void createGroup(const std::string & groupName);
    void removeGroup(const std::string & groupName);
    void loadGroup(const std::string & groupName);
    void unloadGroup(const std::string & groupName);
    //void mergeGroups(const std::string & group1, const std::string & group1, const std::string & newName);

    void addTexture(std::shared_ptr<Texture> tex, 
        const std::string & groupName = "");

private:
    static ResourceManager* s_Instance;
    ResourceManager();
    ~ResourceManager();

    bool reduceLoad();

    ResourceGroup* m_ActiveGroup;
    bool m_Caching;
    static unsigned int s_Timestamp;
    const unsigned long long m_MaxLoad;
    unsigned long long m_CurrentLoad;

    //std::map<std::string, ResourceGroup> m_Loaded;
    std::map<std::string, ResourceGroup> m_Groups;
    std::map<std::string, std::shared_ptr<Texture>> m_Textures;
};

#endif // _ResourceManager_H_