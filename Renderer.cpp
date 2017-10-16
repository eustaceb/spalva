//
// Created by eustace on 18/05/17.
//

#include "Renderer.h"

#include <iostream>
#include <cmath>

#include "imgui/imgui.h"
#include "imgui/Imgui_Impl.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>

#include "resourcing/Shader.h"
#include "renderables/Renderable.h"
#include "scenes/Scene.h"

#include "GUI.h"

#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(int screenWidth, int screenHeight, const char *title)
    : m_Window(nullptr), m_WireframeMode(false), m_ShowUI(true), m_Scene(0)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    m_Window = glfwCreateWindow(screenWidth, screenHeight, title, nullptr, nullptr);
    if (m_Window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_Window);
	
    //glewExperimental = GL_TRUE;
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
    ImGui_Init(m_Window, true);

    int windowWidth, windowHeight;
    glfwGetFramebufferSize(m_Window, &windowWidth, &windowHeight);

    glViewport(0, 0, windowWidth, windowHeight);
}

Renderer::~Renderer()
{
    glfwTerminate();
}

void Renderer::render(glm::mat4 &projection, glm::mat4 &view)
{
    m_Scene->render(projection, view);
    /*GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "OpenGL error #" << std::hex << err << "\n";
    }*/
    if (m_ShowUI)
    {
        ImGui_NewFrame();

        // Scene loader
        const float width = 300, height = 50;
        ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_Once);
        ImGui::SetNextWindowPos(ImVec2(GUI::MARGIN, GUI::MARGIN), ImGuiSetCond_Once);
        ImGui::Begin("Scene loader");
        
        
        static int index = 0;
        static int lastIndex = 0;
        const char* title = "Select scene";
        ImGui::Combo(title, &index, m_SceneLabels);

        if (m_WireframeMode) // Temporarily turn off wireframe for GUI
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        m_Scene->renderGUI(); // Some UI might be scene-dependant
        ImGui::Render();

        if (m_WireframeMode) // Turn it back on
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        if (index != lastIndex) // Change scene if index was changed
        {
            m_Scene->deactivate();
            m_Scene = m_Scenes[m_SceneLabels[index]];
            m_Scene->activate();
            lastIndex = index;
        }
    }
    glfwSwapBuffers(m_Window);
}

void Renderer::update(GLfloat deltaTime)
{
    m_Scene->update(deltaTime);
}

void Renderer::addScene(std::shared_ptr<Scene> scene)
{
    m_Scenes.insert(std::make_pair(scene->getName(), scene));
    m_SceneLabels.push_back(scene->getName());
}

void Renderer::setScene(const std::string &name)
{
    if (m_Scene) m_Scene->deactivate();
    m_Scene = m_Scenes[name];
    m_Scene->activate();
}

GLFWwindow *Renderer::getWindow() const
{
    return m_Window;
}

void Renderer::toggleUI()
{
    m_ShowUI = !m_ShowUI;
    if (m_ShowUI)
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Renderer::toggleWireframe()
{
    m_WireframeMode = !m_WireframeMode;
    if (m_WireframeMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

