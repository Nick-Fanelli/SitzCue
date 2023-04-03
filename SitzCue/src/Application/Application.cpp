#include "Application.h"

using namespace SitzCue;

void Application::SetProject(const Project& project) {

    if(m_ActiveProject != nullptr) {
        // TODO: Handle Saving The Project
        delete m_ActiveProject;
    }

    m_ActiveProject = new Project(project);

    m_ImGuiLayer.GetWindowManager().SetScene<EditorScene>();
}

void Application::StartApplication() {
    m_Display.CreateDisplay();
    m_Display.StartApplicationLoop(this);
}

void Application::OnCreate() {
    m_ImGuiLayer = ImGuiLayer(this);
    m_ImGuiLayer.OnCreate();
}

void Application::OnUpdate(float deltaTime) {
    m_ImGuiLayer.Begin();
    m_ImGuiLayer.End();
}

void Application::OnDestroy() {
    m_ImGuiLayer.OnDestroy();

    if(m_ActiveProject != nullptr) {
        // TODO: Save Project
        delete m_ActiveProject;
    }
}