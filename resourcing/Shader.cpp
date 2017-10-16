//
// Created by eustace on 02/04/17.
//

#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

#include <glm/detail/type_mat.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
    // Load shader code
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::fstream::failure e) {
        std::cout << "ERROR::SHADER::COULD_NOT_READ_FILE" << std::endl;
		std::cout << "Vertex:" << vertexPath << std::endl;
		std::cout << "Fragment:" << fragmentPath << std::endl;
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    // Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
    }
    // Fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
    }
    // Link
    this->program = glCreateProgram();
    glAttachShader(this->program, vertex);
    glAttachShader(this->program, fragment);
    glLinkProgram(this->program);
    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->program, 512, NULL, infoLog);
        std::cout << "ERR::SHADER::PROGRAM::LINKING_FAILED" << std::endl << infoLog << std::endl;
    }

    // Parse all user-settable uniform values for the shader configuration GUI
    auto vShaderParams = parseParams(vShaderCode);
    auto fShaderParams = parseParams(fShaderCode);
    m_UserSettableUniforms.reserve(vShaderParams.size() + fShaderParams.size());
    m_UserSettableUniforms.insert(m_UserSettableUniforms.end(), vShaderParams.begin(), vShaderParams.end());
    m_UserSettableUniforms.insert(m_UserSettableUniforms.end(), fShaderParams.begin(), fShaderParams.end());

    // Cleanup
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const {
    glUseProgram(this->program);
}

GLboolean Shader::getBool(const GLchar * name) const
{
    GLint result;
    glGetUniformiv(this->program, glGetUniformLocation(this->program, name), &result);
    return (GLboolean)result;
}

GLfloat Shader::getFloat(const GLchar * name) const
{
    GLfloat result;
    glGetUniformfv(this->program, glGetUniformLocation(this->program, name), &result);
    return result;
}

GLint Shader::getInt(const GLchar * name) const
{
    GLint result;
    glGetUniformiv(this->program, glGetUniformLocation(this->program, name), &result);
    return result;
}

glm::vec3 Shader::getVec3(const GLchar * name) const
{
    GLfloat result[3];
    glGetUniformfv(this->program, glGetUniformLocation(this->program, name), result);
    return glm::vec3(result[0], result[1], result[2]);
}

void Shader::setBool(const GLchar * name, const GLboolean & value)
{
    glUniform1i(glGetUniformLocation(this->program, name), value);
}

void Shader::setFloat(const GLchar * name, const GLfloat & value)
{
    glUniform1f(glGetUniformLocation(this->program, name), value);
}

void Shader::setInt(const GLchar * name, const GLint & value)
{
    glUniform1i(glGetUniformLocation(this->program, name), value);
}

void Shader::setMat4(const GLchar * name, const glm::mat4 & value)
{
    glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const GLchar * name, const glm::vec3 & value)
{
    glUniform3f(glGetUniformLocation(this->program, name), value.x, value.y, value.z);
}

std::vector<std::pair<std::string, std::string>> Shader::getUserSettableUniforms() const
{
    return m_UserSettableUniforms;
}

// Parses user-settable uniform names in shader code
std::vector<std::pair<std::string, std::string>> Shader::parseParams(const GLchar * shaderCode)
{
    std::vector<std::string> allowedTypes = { "vec3", "float", "bool", "int" };
    std::vector<std::string> skipNames = { "model", "view", "projection", 
        "lightPos", "cameraPos", "lightSpaceMatrix" };

    std::vector<std::pair<std::string, std::string>> result;
    std::string codeStr(shaderCode);

    std::regex uniformRegex("uniform \\w+ \\w+;");

    auto codeBegin = std::sregex_iterator(codeStr.begin(), codeStr.end(), uniformRegex);
    auto codeEnd = std::sregex_iterator();
    for (std::sregex_iterator i = codeBegin; i != codeEnd; i++)
    {
        std::string uniform = (*i).str();
        // Returns a pair where first = type, second = name
        auto parsedUniform = parseUniform(uniform);
        // Check if the uniform type is 'workable' and whether its name is not to be skipped
        bool typeAllowed = (std::find(allowedTypes.begin(), allowedTypes.end(), parsedUniform.first) != allowedTypes.end());
        bool skipName = (std::find(skipNames.begin(), skipNames.end(), parsedUniform.second) != skipNames.end());
        if (typeAllowed && !skipName)
            result.push_back(parsedUniform);
    }
    return result;
}

std::pair<std::string, std::string> Shader::parseUniform(const std::string &uniform)
{
    // Skip 'uniform' part
    size_t begin = uniform.find(" ") + 1;
    size_t end = uniform.find(" ", begin);

    // Parse type
    std::string type = uniform.substr(begin, end - begin);

    begin = end + 1;
    end = uniform.find(";", end);

    // Parse name
    std::string name = uniform.substr(begin, end - begin);

    return std::pair<std::string, std::string>(type, name);
}

GLint Shader::getUniform(const GLchar *name) const {
    return glGetUniformLocation(this->program, name);
}
