//
// Created by eustace on 02/04/17.
//

#ifndef _Shader_H_
#define _Shader_H_

#define GLEW_STATIC
#define GLEW_NO_GLU

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Shader {
public:
    GLuint program;
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    GLint getUniform(const GLchar* name) const;
    void use() const;

    GLboolean getBool(const GLchar* name) const;
    GLfloat getFloat(const GLchar* name) const;
    GLint getInt(const GLchar* name) const;
    glm::vec3 getVec3(const GLchar* name) const;

    void setBool(const GLchar* name, const GLboolean &value);
    void setFloat(const GLchar* name, const GLfloat &value);
    void setInt(const GLchar* name, const GLint &value);
    void setMat4(const GLchar* name, const glm::mat4 &value);
    void setVec3(const GLchar* name, const glm::vec3 &value);

    std::vector<std::pair<std::string, std::string>> getUserSettableUniforms() const;
private:
    // Used for parsing shader code for user settable uniforms to be displayed in GUI
    std::vector<std::pair<std::string, std::string>> parseParams(const GLchar* shaderCode);
    std::pair<std::string, std::string> parseUniform(const std::string &uniform);
    // <Uniform name, type>
    std::vector<std::pair<std::string, std::string>> m_UserSettableUniforms;
};


#endif //_Shader_H_
