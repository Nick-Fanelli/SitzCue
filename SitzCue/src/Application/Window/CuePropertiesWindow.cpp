#include "CuePropertiesWindow.h"

#include "Application/ImGuiDefaults.h"

using namespace SitzCue;

void CuePropertiesWindow::OnUpdate(CueList& cueList, const std::vector<UUID>& selectedCues) {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::Begin("Cue Properties");

    if(selectedCues.size() <= 0) {
        ImGui::Text("No Cue Selected");
    } else if(selectedCues.size() > 1) {
        ImGui::Text("Please Select Just One Cue");
    } else {

        Cue* cue = cueList.GetCue(selectedCues[0]);

        if(cue != nullptr)
            DrawCue(*cue);
    }

    ImGui::End();

}

void CuePropertiesWindow::DrawCue(Cue& cue) {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::Text("Controls");
    ImGui::AlignTextToFramePadding();
    ImGui::SameLine();

    if(ImGui::Button("Move Cue Up")) {}

    ImGui::SameLine();

    if(ImGui::Button("Move Cue Down")) {}

    ImGui::NewLine();

    ImGuiDefaults::DrawTextInput("Cue Name", cue.CueName);
    ImGuiDefaults::DrawCueNumber("Cue Number", cue.CueNumber);
}