#include "Application.h"

#include "ApplicationCache.h"

using namespace SitzQ;

void Application::SetProject(Project* project) {

    if(m_ActiveProject != nullptr) {
        m_ActiveProject->SaveProject();
        delete m_ActiveProject;
    }

    m_ActiveProject = project;

    m_ImGuiLayer.GetWindowManager().SetScene<EditorScene>();

    ApplicationCache::Push(ApplicationCache::Keys::LastActiveProject, project->GetProjectDirectoryPath());
}

void Application::StartApplication() {
    ApplicationCache::Pull();

    m_Display.CreateDisplay();
    m_Display.StartApplicationLoop(this);
}

void Application::OnCreate() {
    m_ImGuiLayer = ImGuiLayer(this);
    m_ImGuiLayer.OnCreate();

    auto lastActiveProject = ApplicationCache::Get(ApplicationCache::Keys::LastActiveProject);

    if(lastActiveProject.has_value() && FileUtils::Exists(lastActiveProject.value())) {
        SetProject(new Project(lastActiveProject.value()));
    }

    // TODO: REMOVE DEMO CODE
    // Project* project = new Project("/Users/nickfanelli/Desktop/Example Project.sitzqprj");

    // project->GetCueList().CreateCue("House Open (Pre-Show)", 1.0f);
    // project->GetCueList().CreateCue("Pre-Show Announcement", 2.0f);
    // project->GetCueList().CreateCue("Show Mode", 3.0f);
    // project->GetCueList().CreateCue("House Open (Post-Show)", 4.0f);

    // SetProject(project);

}

void Application::OnUpdate(float deltaTime) {
    m_ImGuiLayer.Begin();
    m_ImGuiLayer.End();
}

void Application::OnDestroy() {

    ApplicationCache::Commit();

    m_ImGuiLayer.OnDestroy();

    if(m_ActiveProject != nullptr) {
        m_ActiveProject->SaveProject();
        delete m_ActiveProject;
    }
}