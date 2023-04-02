#include "CuePropertiesWindow.h"

#include "Application/ImGuiDefaults.h"

using namespace SitzCue;

CuePropertiesWindow::CuePropertiesWindow(CueListWindow* cueListWindowPtr) {
    m_CueListWindowPtr = cueListWindowPtr;
}

void CuePropertiesWindow::OnUpdate() {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::Begin("Cue Properties");

    auto& selectedCues = m_CueListWindowPtr->GetSelectedCues();

    if(selectedCues.size() <= 0) {
        ImGui::Text("No Cue Selected");
    } else if(selectedCues.size() > 1) {
        ImGui::Text("Please Select Just One Cue");
    } else {
        DrawCue(m_CueListWindowPtr->GetCueList().GetCue(selectedCues[0]));
    }

    ImGui::End();

}

void CuePropertiesWindow::DrawCue(Cue& cue) {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::Text("Controls");
    ImGui::AlignTextToFramePadding();
    ImGui::SameLine();

    // if(ImGui::Button("Go Up")) {    }

    // ImGui::SameLine();

    // if(ImGui::Button("Go Down"));

    ImGui::NewLine();

    ImGuiDefaults::DrawTextInput("Cue Name", cue.CueName);
    ImGuiDefaults::DrawCueNumber("Cue Number", cue.CueNumber);
}