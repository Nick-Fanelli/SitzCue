#include "Window.h"

#include "CueListWindow.h"
#include "CuePropertiesWindow.h"

#include "Application/Application.h"
#include "Project/Project.h"

using namespace SitzCue;


// ===================================================================================================================
// Window
// ===================================================================================================================

void Window::OnUpdate() {}

// ===================================================================================================================
// Scene
// ===================================================================================================================

void Scene::OnCreate() {}
void Scene::OnUpdate() {}
void Scene::OnDestroy() {}


// ===================================================================================================================
// Lander Scene
// ===================================================================================================================

void LanderScene::OnCreate() {
    SITZCUE_PROFILE_FUNCTION();

}

void LanderScene::OnUpdate() {
    SITZCUE_PROFILE_FUNCTION();

    static constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | 
                                                    ImGuiWindowFlags_NoCollapse | 
                                                    ImGuiWindowFlags_NoMove     |
                                                    ImGuiWindowFlags_NoResize;
    static bool shouldDisplay = true;

    static ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y));
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::Begin("Lander", &shouldDisplay, windowFlags);

    if(ImGui::Button("Create Project"))
        Project::CreateNewProject(m_WindowManagerPtr->GetApplicationPtr());

    if(ImGui::Button("Open Project")) {
        Project::OpenProjectDialog(m_WindowManagerPtr->GetApplicationPtr());
    }

    ImGui::End();

}

void LanderScene::OnDestroy() {
    SITZCUE_PROFILE_FUNCTION();

}

// ===================================================================================================================
// Editor Scene
// ===================================================================================================================

void EditorScene::OnCreate() {
    SITZCUE_PROFILE_FUNCTION();

    if(m_WindowManagerPtr->GetApplicationPtr()->GetActiveProject() == nullptr) {
        Log::Error("No Project Found");
        m_WindowManagerPtr->SetScene<LanderScene>();
    }

    m_StatusBarPtr = new StatusBar(m_WindowManagerPtr->GetApplicationPtr());
    m_CueListWindowPtr = new CueListWindow(m_WindowManagerPtr->GetApplicationPtr());
    m_CuePropertiesWindowPtr = new CuePropertiesWindow(m_CueListWindowPtr);
}

void EditorScene::OnUpdate() {
    SITZCUE_PROFILE_FUNCTION();

    m_StatusBarPtr->OnUpdate();
    m_CueListWindowPtr->OnUpdate();
    m_CuePropertiesWindowPtr->OnUpdate();
}

void EditorScene::OnDestroy() {
    SITZCUE_PROFILE_FUNCTION();

    delete m_StatusBarPtr;
    delete m_CueListWindowPtr;
    delete m_CuePropertiesWindowPtr;
}

// ===================================================================================================================
// Window Manager
// ===================================================================================================================

void WindowManager::OnUpdate() {
    if(m_ActiveScene != nullptr)
        m_ActiveScene->OnUpdate();
}

void WindowManager::OnDestroy() {
    if(m_ActiveScene != nullptr)
        m_ActiveScene->OnDestroy();
}