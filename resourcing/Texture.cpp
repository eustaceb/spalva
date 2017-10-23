#include "Texture.h"
#include "../Helpers.h"

#include <iostream>
#include <stb/stb_image.h>


Texture::Texture(const std::string & path, const std::string & type, 
    const bool & gammaCorrected, const std::string & label)
    : Resource(ResourceType::REGULAR, path),
    m_Type(type), m_Label(label), m_Format(GL_NONE),
    m_InternalFormat(GL_NONE), m_GammaCorrected(gammaCorrected)
{

}

Texture::Texture(const std::string & path, const std::string & type, 
    const GLenum & format, const GLenum & internalFormat, 
    const std::string & label)
    : Resource(ResourceType::REGULAR, path), 
    m_Type(type), m_Format(format), m_GammaCorrected(false),
    m_InternalFormat(internalFormat), m_Label(label)
{

}

Texture::~Texture()
{
    // Safeguard in case something went wrong and the count wasn't decreased
    assert(m_RefCount == 0);
    if (m_RefCount != 0)
        unload();
}

void Texture::load()
{
    glGenTextures(1, &m_Id);
    unsigned char* image = stbi_load(m_Path.c_str(), &m_Width, &m_Height, &m_Components, STBI_rgb);
    if (image)
    {
        // Auto determine the format
        if (m_Format == GL_NONE || m_InternalFormat == GL_NONE)
        {
            if (m_Components == 1)
            {
                m_InternalFormat = GL_RED;
                m_Format = GL_RED;
            }
            else if (m_Components == 3)
            {
                m_InternalFormat = m_GammaCorrected ? GL_SRGB : GL_RGB;
                m_Format = GL_RGB;
            }
            else if (m_Components == 4)
            {
                m_InternalFormat = m_GammaCorrected ? GL_SRGB_ALPHA : GL_RGBA;
                m_Format = GL_RGBA;
            }
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
    else std::cout << "ERR::Texture::loadTexture::Texture failed to load at path: " << m_Path << "\n";

    stbi_image_free(image);
}

void Texture::unload()
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

std::string Texture::getLabel() const
{
    return m_Label;
}

std::string Texture::getType() const
{
    return m_Type;
}