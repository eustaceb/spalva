#pragma once
#include <assert.h>

class Resource
{
public:
    Resource() {};
    virtual ~Resource() {};

    // @TODO: Such loading/unloading is only relevant for unique resources
    virtual void load() { m_Count++; };
    virtual void unload() 
    {
        assert(m_Count > 0);
        if (m_Count > 0) 
            m_Count--;
    };
    virtual unsigned long long getMemoryTaken() { return m_MemoryTaken; }
protected:
    unsigned long long m_MemoryTaken;
    unsigned int m_Count = 0;
};

