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

    m_ImGuiLayer.GetSceneManager().SetScene<EditorScene>();

    ApplicationCache::Push(ApplicationCache::Keys::LastActiveProject, project->GetProjectDirectoryPath());

    AssetManager::AssignWatchDirectory(m_ActiveProject->GetAssetsDirectoryPath());
}

// Starts and Blocks the Program Until Finished
void Application::StartApplication() { // Main method to start the application

    SITZCUE_PROFILE_FUNCTION();

    Log::Trace("Creating the Application");

    Log::Trace("NFD Init");
    NFD_Init(); // Init Native File Dialog

    Log::Trace("Application Cache Pull");
    ApplicationCache::Pull(); // Pull the Application Cache

    Log::Trace("Creating the display");
    m_Display.CreateDisplay(); // Create the GLFW window/display

    Log::Trace("Starting application loop");
    m_Display.StartApplicationLoop(this); // Start the loop and give it the application to start
}

void Application::OnCreate() {

    SITZCUE_PROFILE_FUNCTION();

    // Create the ImGui Layer for GUI
    m_ImGuiLayer = ImGuiLayer(this);
    m_ImGuiLayer.OnCreate();

    // Load the cached last active project
    auto lastActiveProject = ApplicationCache::Get(ApplicationCache::Keys::LastActiveProject);
    if(lastActiveProject.has_value() && FileUtils::Exists(lastActiveProject.value())) {
        SetProject(new Project(lastActiveProject.value()));
    }
}

void Application::OnUpdate(float deltaTime) {
    
    SITZCUE_PROFILE_FUNCTION();

    // Draw the ImGui Layer
    m_ImGuiLayer.Begin();
    m_ImGuiLayer.End();
}

void Application::OnDestroy() {

    SITZCUE_PROFILE_FUNCTION();

    ApplicationCache::Commit(); // Save the application cache

    m_ImGuiLayer.OnDestroy(); // Destroy the ImGui Layer

    // Delete and save the active project
    if(m_ActiveProject != nullptr) {
        m_ActiveProject->SaveProject();
        delete m_ActiveProject;
    }

    Log::Trace("NFD Quit");
    NFD_Quit(); // Quit the Native File Dialog API

    Log::Info("Application Destroyed");
}