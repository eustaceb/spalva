#pragma once
#include <cassert>
#include <string>

enum ResourceType {
    REGULAR, // Resources with the same path are treated as duplicates
    UNIQUE,  // Each resource is unique, even if the path is identical
    MOLD // One resource serves as template for multiple unique resources
};

class Resource
{
public:
    Resource(const ResourceType & type, const std::string & path);
    virtual ~Resource() {};

    virtual void load() = 0;
    virtual void unload() = 0;
    void refer();
    void derefer();

    unsigned int getRefCount() const;
    // Note: Resource types are not yet implemented
    void setType(const ResourceType & type);
    // Note: Resource types are not yet implemented
    ResourceType getType() const { return m_Type; }

    void setPath(const std::string & path);
    std::string getPath() const;

    void setResourceID(const unsigned int & id);
    unsigned int getResourceID() const;
    unsigned long long getMemoryTaken();

protected:
    ResourceType m_Type;
    // Multiple paths will be split by ':', e.g. for vertex/fragment shaders
    std::string m_Path;
    unsigned int m_ResourceID;
    unsigned long long m_MemoryTaken;
    unsigned int m_RefCount;
};

