#pragma once
#ifndef _Texture_H_
#define _Texture_H_


#include <string>
#include <glad/glad.h>
/**
    This might end up as the base class for Texture2D, Texture3D and Cubemaps in future 
**/
class Texture
{
public:
    Texture(const std::string &path, const std::string &type, 
        const bool &gammaCorrected = false, const std::string &label = "Texture");
    Texture(const std::string &path, const std::string &type,
        const GLenum &format, const GLenum &internalFormat, const std::string &label = "Texture");
    ~Texture();

    void bind();
    void unbind();

    GLuint getId() const;
    std::string getPath() const;
    std::string getLabel() const;
    std::string getType() const;
private:
    void loadTexture(const std::string &path, const bool &gammaCorrected = false);
    void loadTexture(const std::string & path, const GLenum & format, const GLenum & internalFormat);

    GLuint m_Id;
    std::string m_Path;
    std::string m_Label;
    std::string m_Type;

    GLenum m_Format, m_InternalFormat;
    GLint m_Width, m_Height, m_Components;
};

#endif // _Texture_H_