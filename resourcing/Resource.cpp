#include "Resource.h"
#include "../Helpers.h"

#include <sstream>

Resource::Resource(const ResourceType & type, const std::string & path)
    : m_Type(type), m_Path(path), m_RefCount(0), m_MemoryTaken(0)
{
}

void Resource::refer() 
{ 
    m_RefCount++;
}

void Resource::derefer()
{
    assert(m_RefCount > 0);
    if (m_RefCount > 0)
        m_RefCount--;
}

unsigned int Resource::getRefCount() const
{
    return m_RefCount;
}

void Resource::setType(const ResourceType & type)
{
    m_Type = type;
}

void Resource::setPath(const std::string & path)
{
    m_Path = path;
}

std::string Resource::getPath() const
{
    return m_Path;
}

void Resource::setResourceID(const unsigned int & id)
{
    m_ResourceID = id;
}

unsigned int Resource::getResourceID() const
{
    return m_ResourceID;
}

unsigned long long Resource::getMemoryTaken()
{
    // Lazy loading
    if (m_MemoryTaken == 0)
    {
        assert(m_Path != "");
        std::stringstream ss(m_Path);
        std::string path;
        // For resources with multiple sources
        while (ss.good())
        {
            getline(ss, path, ':');
            m_MemoryTaken += Helpers::fileSize(path);
        }
    }
    return m_MemoryTaken;
}