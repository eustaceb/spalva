//
// Created by eustace on 19/05/17.
//

#ifndef _LightCube_H_
#define _LightCube_H_

#include <memory>
#include <vector>
#include "Renderable.h"
#include "LightModel.h"
#include <glad/glad.h>

class Shader;

class LightCube : public Renderable {
public:
    LightCube(std::shared_ptr<Shader> shader, const glm::vec3 &pos, const GLuint &index,
        const std::string &label = "LightCube");
    ~LightCube();

    void render();

    glm::vec3 getAmbient();
    glm::vec3 getDiffuse();
    glm::vec3 getSpecular();
    void setAmbient(glm::vec3 ambient);
    void setDiffuse(glm::vec3 diffuse);
    void setSpecular(glm::vec3 specular);

    void addObserver(std::weak_ptr<LightModel> m);

    GLfloat getConstant();
    void setConstant(GLfloat constant);
    GLfloat getLinear();
    void setLinear(GLfloat linear);
    GLfloat getQuadratic();
    void setQuadratic(GLfloat quadratic);
private:
    GLuint m_VBO, m_VAO;
    //std::vector<GLuint> m_Indices;
    std::vector<std::weak_ptr<LightModel>> m_Observers;
    GLuint m_Index;

    glm::vec3 m_Ambient, m_Diffuse, m_Specular, m_Color;
    GLfloat m_Constant, m_Linear, m_Quadratic;
    void mixColors();
    void notifyAll();
};


#endif //_LightCube_H_
