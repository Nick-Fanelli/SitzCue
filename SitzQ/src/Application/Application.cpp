#include "Application.h"

#include "ApplicationCache.h"

#include "Runtime/RuntimeEngine.h"
#include "Runtime/Asset.h"

using namespace SitzQ;

void Application::SetProject(Project* project) {

    if(m_ActiveProject != nullptr) {
        m_ActiveProject->SaveProject();
        delete m_ActiveProject;
    }

    m_ActiveProject = project;

    m_ImGuiLayer.GetWindowManager().SetScene<EditorScene>();

    ApplicationCache::Push(ApplicationCache::Keys::LastActiveProject, project->GetProjectDirectoryPath());

    AssetManager::AssignWatchDirectory(m_ActiveProject->GetAssetsDirectoryPath());
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