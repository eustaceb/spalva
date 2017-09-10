#pragma once
#ifndef _Helpers_H_
#define _Helpers_H_

/*#include <string>
#include <iostream>
#include <stb/stb_image.h>
#include <glad/glad.h>*/

namespace Helpers {
/*
    inline GLuint loadTexture(const std::string &path, const bool &gammaCorrected = false)
    {
        GLuint texLoc;
        glGenTextures(1, &texLoc);

        int width, height, components;
        unsigned char* image = stbi_load(path.c_str(), &width, &height, &components, STBI_rgb);
        if (image)
        {
            GLenum internalFormat, format;
            if (components == 1)
            {
                internalFormat = GL_RED;
                format = GL_RED;
            }
            else if (components == 3)
            {
                internalFormat = gammaCorrected ? GL_SRGB : GL_RGB;
                format = GL_RGB;
            }
            else if (components == 4)
            {
                internalFormat = gammaCorrected ? GL_SRGB_ALPHA : GL_RGBA;
                format = GL_RGBA;
            }

            glBindTexture(GL_TEXTURE_2D, texLoc);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(GL_TEXTURE_2D);
            bool clamp = (format == GL_RGBA || format == GL_SRGB_ALPHA);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else std::cout << "ERR::Helpers::loadTexture::Texture failed to load at path: " << path << "\n";

        stbi_image_free(image);
        return texLoc;
    }*/
}

#endif // _Helpers_H_