#include "CuePropertiesWindow.h"

#include "Application/ImGuiDefaults.h"

using namespace SitzCue;

CuePropertiesWindow::CuePropertiesWindow(CueListWindow* cueListWindowPtr) {
    m_CueListWindowPtr = cueListWindowPtr;
}

void CuePropertiesWindow::OnUpdate() {

    // std::vector<Cue*>& selectedCues = m_CueListWindowPtr->GetSelectedCues();

    ImGui::Begin("Cue Properties");

    // if(selectedCues.size() <= 0) {
    //     ImGui::Text("No Cue Selected");
    // } else {
    //     DrawCue(*selectedCues[0]);
    // }

    ImGui::End();

}

void CuePropertiesWindow::DrawCue(Cue& cue) {
    ImGui::Text("Controls");
    ImGui::AlignTextToFramePadding();
    ImGui::SameLine();

    // if(ImGui::Button("Go Up")) {    }

    // ImGui::SameLine();

    // if(ImGui::Button("Go Down"));

    ImGui::NewLine();

    ImGuiDefaults::DrawTextInput("Cue Name", cue.CueName);
    ImGuiDefaults::DrawFloat("Cue Number", cue.CueNumber);
}