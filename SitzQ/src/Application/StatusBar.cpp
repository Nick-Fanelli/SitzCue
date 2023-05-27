#include "StatusBar.h"

#include "Application/Application.h"

#include "Runtime/Audio.h"

using namespace SitzQ;

static constexpr ImVec4 StatusBarColor = { 0.06f, 0.06f, 0.06f, 1.0f };

static constexpr ImGuiWindowFlags s_WindowFlags = 
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoDocking;

static bool s_IsVisible = true;

void StatusBar::OnUpdate() {

    SITZCUE_PROFILE_FUNCTION();

    static auto& colors = ImGui::GetStyle().Colors;
    static ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - StatusBarHeight));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, StatusBarHeight));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, StatusBarColor);

    ImGui::Begin("Status Bar", &s_IsVisible, s_WindowFlags);

    ImGui::Text("%s", m_ApplicationPtr->GetActiveProject()->GetProjectName().c_str());

    auto& audioLevel = AudioEngine::GetMasterAudioLevel();

    ImGui::Text("Left, %g", audioLevel.first);
    ImGui::Text("Left, %g", audioLevel.second);

    ImGui::End();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();

}