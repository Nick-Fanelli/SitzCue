#include "Application.h"

#include <nfd.hpp>

#include "ApplicationCache.h"

#include "Runtime/RuntimeEngine.h"
#include "Runtime/Asset.h"

using namespace SitzQ;

void Application::SetProject(Project* project) {

    SITZCUE_PROFILE_FUNCTION();

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

    SITZCUE_PROFILE_FUNCTION();

    Log::Trace("Creating the Application");

    Log::Trace("NFD Init");
    NFD_Init();

    Log::Trace("Application Cache Pull");
    ApplicationCache::Pull();

    Log::Trace("Creating the display");
    m_Display.CreateDisplay();

    Log::Trace("Starting application loop");
    m_Display.StartApplicationLoop(this);
}

void Application::OnCreate() {

    SITZCUE_PROFILE_FUNCTION();

    m_ImGuiLayer = ImGuiLayer(this);
    m_ImGuiLayer.OnCreate();

    auto lastActiveProject = ApplicationCache::Get(ApplicationCache::Keys::LastActiveProject);

    if(lastActiveProject.has_value() && FileUtils::Exists(lastActiveProject.value())) {
        SetProject(new Project(lastActiveProject.value()));
    }
}

void Application::OnUpdate(float deltaTime) {
    
    SITZCUE_PROFILE_FUNCTION();

    m_ImGuiLayer.Begin();
    m_ImGuiLayer.End();
}

void Application::OnDestroy() {

    SITZCUE_PROFILE_FUNCTION();

    ApplicationCache::Commit();

    m_ImGuiLayer.OnDestroy();

    if(m_ActiveProject != nullptr) {
        m_ActiveProject->SaveProject();
        delete m_ActiveProject;
    }

    Log::Trace("NFD Quit");
    NFD_Quit();

    Log::Info("Application Destroyed");
}