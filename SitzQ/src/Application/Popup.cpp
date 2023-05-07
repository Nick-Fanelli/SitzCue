#include "Popup.h"

#include "ImGuiDefaults.h"

using namespace SitzCue;

static constexpr ImGuiWindowFlags s_RigidWindowFlags =  ImGuiWindowFlags_NoTitleBar |
                                                        ImGuiWindowFlags_NoResize | 
                                                        ImGuiWindowFlags_NoMove | 
                                                        ImGuiWindowFlags_NoDocking |
                                                        ImGuiWindowFlags_NoSavedSettings;

static void DrawBackgroundBlur() {

    SITZCUE_PROFILE_FUNCTION();

    static ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y));

    ImGui::SetNextWindowBgAlpha(0.5f);
    ImGui::Begin("Background Blur", NULL, s_RigidWindowFlags | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImVec2 windowPosition = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();

    drawList->AddRectFilled(windowPosition, ImVec2(windowPosition.x + windowSize.x, windowPosition.y + windowSize.y), ImColor(0, 0, 0, 128));

    ImGui::End();

}   

static void BeginRigidWindow(bool* opened) {

    static ImGuiViewport* viewport = ImGui::GetMainViewport();

    static constexpr ImVec2 windowSize{ 400.0f, 500.0f };
    static constexpr float windowWidthHalfSize = windowSize.x / 2.0f;
    static constexpr float windowHeightHalfSize = windowSize.y / 2.0f;

    ImGui::SetNextWindowPos(ImVec2((viewport->Pos.x + viewport->Size.x / 2.0f) - windowWidthHalfSize, (viewport->Pos.y + viewport->Size.y / 2.0f) - windowHeightHalfSize));
    ImGui::SetNextWindowSize(windowSize);

    ImGui::Begin("Rigid Popup", opened, s_RigidWindowFlags);
}

void ConfirmationPopupWindow::Update() {

    DrawBackgroundBlur();

    BeginRigidWindow(&m_IsActive);

    ImGui::PushFont(ImGuiLayer::GetBoldFont());
    float headerTextWidth = ImGui::CalcTextSize(m_HeaderText.c_str()).x;

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - headerTextWidth) * 0.5f);
    ImGui::SetCursorPosY(10.0f);

    ImGui::Text("%s", m_HeaderText.c_str());
    ImGui::PopFont();

    ImGui::SetCursorPosY(200.0f);

    ImGui::TextWrapped("%s", m_Message.c_str());

    static constexpr ImVec2 buttonSize{ 185.0f, 30.0f };

    ImGui::SetCursorPosX(10.0f);
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - buttonSize.y - 10.0f);

    ImGui::Button("Cancel", buttonSize);

    ImGui::SameLine();
    ImGui::SetCursorPosX(205.0f);

    ImGui::PushStyleColor(ImGuiCol_Button, ImGuiDefaults::AccentColor);
    ImGui::Button("Okay", buttonSize);
    ImGui::PopStyleColor();

    ImGui::End();

}

void Popup::Update() {

    if(m_PopupWindow && m_PopupWindow->IsActive()) {
        m_PopupWindow->Update();
    }
        

}