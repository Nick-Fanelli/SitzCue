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
        ImGuiWindowFlags_NoTitleBar;

static bool s_IsVisible = true;

static void DrawAudioFader(float fadeValue, ImVec2 faderSize) {
    SITZCUE_PROFILE_FUNCTION();

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize(faderSize, ImGui::CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);

    ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, 0))
        return;

    ImGui::RenderFrame(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    float s = size.y - (size.y * (1.0f - fadeValue));

    drawList->AddRectFilled(pos + ImVec2{ 0.0f, size.y * (1.0f - fadeValue) }, pos + size, IM_COL32(0, 255, 0, 255), size.y - (size.y * (1.0f - fadeValue)) < style.FrameRounding ? 0 : style.FrameRounding);

}

static bool s_IsVUIMeterVisible = true;

static void DrawMasterAudioLevels() {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::Begin("VU Meter", &s_IsVUIMeterVisible, ImGuiWindowFlags_NoTitleBar);

    ImVec2 faderSize = (ImGui::GetContentRegionAvail() * ImVec2{ 0.5f, 1.0f }) - ImVec2{ 5.0f, 0.0f };

    auto& audioLevel = AudioEngine::GetMasterAudioLevel();

    DrawAudioFader(audioLevel.first, faderSize);
    ImGui::SameLine();
    DrawAudioFader(audioLevel.second, faderSize);

    ImGui::End();

}

void StatusBar::OnUpdate() {

    SITZCUE_PROFILE_FUNCTION();

    // DrawMasterAudioLevels();

    static auto& colors = ImGui::GetStyle().Colors;
    static ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - StatusBarHeight));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, StatusBarHeight));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, StatusBarColor);

    ImGui::Begin("Status Bar", &s_IsVisible, s_WindowFlags);

    ImGui::Text("%s", m_ApplicationPtr->GetActiveProject()->GetProjectName().c_str());

    ImGui::End();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();

}