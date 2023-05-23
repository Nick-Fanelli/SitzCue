#include "Window.h"

#include "CueListWindow.h"
#include "CuePropertiesWindow.h"
#include "AssetBrowswerWindow.h"

#include "Application/Application.h"
#include "Project/Project.h"

using namespace SitzQ;


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

static inline void DrawLanderPage(WindowManager* windowManagerPtr) {

    static ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::PushStyleColor(ImGuiCol_Border, { 0.0f, 0.0f, 0.0f, 0.0f });

    // Left Child
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::BeginChild("##left-child", { viewport->Size.x / 2.0f, viewport->Size.y - 10.0f }, true);

    ImGui::AlignTextToFramePadding();
    ImGui::Text("SitzQ");

    ImGui::EndChild();

    ImGui::SetNextWindowPos({ viewport->Pos.x + (viewport->Size.x / 2.0f), viewport->Pos.y });
    ImGui::BeginChild("##right-child", { viewport->Size.x / 2.0f, viewport->Size.y - 10.0f }, true);
    
    if(ImGui::Button("Create Project"))
        Project::CreateNewProject(windowManagerPtr->GetApplicationPtr());

    if(ImGui::Button("Open Project")) {
        Project::OpenProjectDialog(windowManagerPtr->GetApplicationPtr());
    }

    ImGui::EndChild();

    ImGui::PopStyleColor();


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

    DrawLanderPage(m_WindowManagerPtr);

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
    m_CueListWindowPtr = new CueListWindow();
    m_AssetBrowserWindow = new AssetBrowserWindow(m_WindowManagerPtr->GetApplicationPtr());
}

void EditorScene::OnUpdate() {
    SITZCUE_PROFILE_FUNCTION();

    m_StatusBarPtr->OnUpdate();
    m_CueListWindowPtr->OnUpdate(m_WindowManagerPtr->GetApplicationPtr()->GetActiveProject()->GetCueList());
    m_AssetBrowserWindow->OnUpdate();
}

void EditorScene::OnDestroy() {
    SITZCUE_PROFILE_FUNCTION();

    delete m_StatusBarPtr;
    delete m_CueListWindowPtr;
    delete m_AssetBrowserWindow;
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