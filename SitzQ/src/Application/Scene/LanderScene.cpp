#include "Scene.h"

#include "Project/Project.h"

using namespace SitzQ;

// ===================================================================================================================
// Lander Scene
// ===================================================================================================================
void LanderScene::OnCreate() {
    SITZCUE_PROFILE_FUNCTION();
}

static inline void DrawLanderPage(SceneManager* sceneManagerPtr) {

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
        Project::CreateNewProject(sceneManagerPtr->GetApplicationPtr());

    if(ImGui::Button("Open Project")) {
        Project::OpenProjectDialog(sceneManagerPtr->GetApplicationPtr());
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

    DrawLanderPage(m_SceneManagerPtr);

    ImGui::End();

}

void LanderScene::OnDestroy() {
    SITZCUE_PROFILE_FUNCTION();

}