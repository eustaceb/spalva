#pragma once
#ifndef _InstancedModel_H_
#define _InstancedModel_H_

#include "Model.h"

class InstancedModel :
    public Model
{
public:
    InstancedModel(std::shared_ptr<Shader> shader, const glm::vec3 &pos,
        const GLchar *path, const unsigned int &count, const std::vector<glm::mat4> &modelMatrices,
        const std::string &label = "InstancedModel");
    ~InstancedModel();

    void render();
private:
    unsigned int m_Count;
    GLuint m_InstanceMatrixAB;
};
#endif // _InstancedModel_H_

