#include "Texture.h"

#include <iostream>
#include <stb/stb_image.h>


Texture::Texture(const std::string & path, const std::string & type, 
    const bool & gammaCorrected, const std::string & label)
    : m_Path(path), m_Type(type), m_Label(label)
{
    glGenTextures(1, &m_Id);
    loadTexture(path, gammaCorrected);
}

Texture::Texture(const std::string & path, const std::string & type, const GLenum & format, 
    const GLenum & internalFormat, const std::string & label)
    : m_Path(path), m_Type(type), m_Label(label)
{
    glGenTextures(1, &m_Id);
    loadTexture(path, format, internalFormat);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_Id);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_Id);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getId() const
{
    return m_Id;
}

std::string Texture::getPath() const
{
    return m_Path;
}

std::string Texture::getLabel() const
{
    return m_Label;
}

std::string Texture::getType() const
{
    return m_Type;
}

void Texture::loadTexture(const std::string & path, const bool & gammaCorrected)
{
    unsigned char* image = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Components, STBI_rgb);
    if (image)
    {
        if (m_Components == 1)
        {
            m_InternalFormat = GL_RED;
            m_Format = GL_RED;
        }
        else if (m_Components == 3)
        {
            m_InternalFormat = gammaCorrected ? GL_SRGB : GL_RGB;
            m_Format = GL_RGB;
        }
        else if (m_Components == 4)
        {
            m_InternalFormat = gammaCorrected ? GL_SRGB_ALPHA : GL_RGBA;
            m_Format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, m_Id);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, 
            m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        bool clamp = (m_Format == GL_RGBA || m_Format == GL_SRGB_ALPHA);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else std::cout << "ERR::Texture::loadTexture::Texture failed to load at path: " << path << "\n";

    stbi_image_free(image);
}

void Texture::loadTexture(const std::string & path, const GLenum & format, const GLenum & internalFormat)
{
    m_Format = format;
    m_InternalFormat = internalFormat;
    unsigned char* image = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Components, STBI_rgb);
    if (image)
    {
        glBindTexture(GL_TEXTURE_2D, m_Id);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, 
            m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        bool clamp = (m_Format == GL_RGBA || m_Format == GL_SRGB_ALPHA);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else std::cout << "ERR::Texture::loadTexture::Texture failed to load at path: " << path << "\n";

    stbi_image_free(image);
}
