#pragma once
#ifndef _GUI_H_
#define _GUI_H_

#include <vector>

#include "imgui/imgui.h"
#include "imgui/Imgui_Impl.h"

#include "Shader.h"

namespace GUI {
    const static float MARGIN = 10;

    inline void renderShaderOptions(Shader* shader, std::vector<std::pair<std::string, std::string>> uniforms)
    {
        if (uniforms.size() > 0)
        {
            if (ImGui::CollapsingHeader("Shader options"))
            {
                for (auto uniforms_it = uniforms.begin(); uniforms_it != uniforms.end(); uniforms_it++)
                {
                    std::string type = (*uniforms_it).first;
                    std::string name = (*uniforms_it).second;
                    // @TODO: Check if querying values from shaders every time can influence performance.
                    if (type == "vec3")
                    {
                        glm::vec3 vec3Values = shader->getVec3(name.c_str());
                        GLfloat values[3] = { vec3Values.x, vec3Values.y, vec3Values.z };
                        if (ImGui::InputFloat3(name.c_str(), &values[0]))
                        {
                            glm::vec3 valuesVec(values[0], values[1], values[2]);
                            shader->use();
                            shader->setVec3(name.c_str(), valuesVec);
                        }
                    }
                    else if (type == "float")
                    {
                        GLfloat floatValue = shader->getFloat(name.c_str());
                        ImGui::PushItemWidth(140.0f);
                        if (ImGui::InputFloat(name.c_str(), &floatValue))
                        {
                            shader->use();
                            shader->setFloat(name.c_str(), floatValue);
                        }
                        ImGui::PopItemWidth();
                    }
                    else if (type == "int")
                    {
                        ImGui::PushItemWidth(140.0f);
                        GLint intValue = shader->getInt(name.c_str());
                        if (ImGui::InputInt(name.c_str(), &intValue))
                        {
                            shader->use();
                            shader->setInt(name.c_str(), intValue);
                        }
                        ImGui::PopItemWidth();
                    }
                    else if (type == "bool")
                    {
                        bool boolValue = shader->getBool(name.c_str());
                        if (ImGui::Checkbox(name.c_str(), &boolValue))
                        {
                            shader->use();
                            shader->setBool(name.c_str(), boolValue);
                        }
                    }
                }
            }
        }
    }
}

#endif // _GUI_H_