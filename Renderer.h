//
// Created by eustace on 18/05/17.
//

#ifndef _Renderer_H_
#define _Renderer_H_

#include <vector>
#include <map>
#include <memory>

#include <glm/vec4.hpp>
#include <glm/detail/type_mat.hpp>

struct GLFWwindow;
class Renderable;
class Shader;
class VertexBuffer;
class Scene;

class Renderer {

public:
    Renderer(int screenWidth, int screenHeight, const char* title);
    ~Renderer();

    void render(glm::mat4& projection, glm::mat4& view);

    void addScene(const std::string &name, std::shared_ptr<Scene> scene);
    void setScene(const std::string &name);

    void toggleUI();
    void toggleWireframe();

    GLFWwindow* getWindow();

private:
    GLFWwindow* m_Window;
    std::shared_ptr<Scene> m_Scene;

    std::vector<std::string> m_SceneLabels;
    std::map <std::string, std::shared_ptr<Scene>> m_Scenes;

    bool m_ShowUI, m_WireframeMode;

};


#endif //_Renderer_H_
