#include "Application.h"

using namespace SitzCue;

void Application::SetProject(Project* project) {

    if(m_ActiveProject != nullptr) {
        m_ActiveProject->SaveProject();
        delete m_ActiveProject;
    }

    m_ActiveProject = project;

    m_ImGuiLayer.GetWindowManager().SetScene<EditorScene>();
}

void Application::StartApplication() {
    m_Display.CreateDisplay();
    m_Display.StartApplicationLoop(this);
}

void Application::OnCreate() {
    m_ImGuiLayer = ImGuiLayer(this);
    m_ImGuiLayer.OnCreate();

    // TODO: REMOVE DEMO CODE
    Project* project = new Project("/Users/nickfanelli/Desktop/Example Project.sitzqprj");

    // project->GetCueList().CreateCue("House Open (Pre-Show)", 1.0f);
    // project->GetCueList().CreateCue("Pre-Show Announcement", 2.0f);
    // project->GetCueList().CreateCue("Show Mode", 3.0f);
    // project->GetCueList().CreateCue("House Open (Post-Show)", 4.0f);

    SetProject(project);
}

void Application::OnUpdate(float deltaTime) {
    m_ImGuiLayer.Begin();
    m_ImGuiLayer.End();
}

void Application::OnDestroy() {
    m_ImGuiLayer.OnDestroy();

    if(m_ActiveProject != nullptr) {
        m_ActiveProject->SaveProject();
        delete m_ActiveProject;
    }
}