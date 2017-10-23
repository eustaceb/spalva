#pragma once
#ifndef _Texture_H_
#define _Texture_H_

#include <string>
#include <glad/glad.h>
#include "Resource.h"

class Texture : public Resource
{
public:
    Texture(const std::string &path, const std::string &type, 
        const bool &gammaCorrected = false, const std::string &label = "Texture");
    Texture(const std::string &path, const std::string &type,
        const GLenum &format, const GLenum &internalFormat, const std::string &label = "Texture");
    ~Texture();

    void load();
    void unload();

    void bind();
    static void unbind();

    GLuint getId() const;
    std::string getLabel() const;
    std::string getType() const;
private:
    GLuint m_Id;
    std::string m_Label;
    std::string m_Type;

    bool m_GammaCorrected;
    GLenum m_Format, m_InternalFormat;
    GLint m_Width, m_Height, m_Components;
};

#endif // _Texture_H_