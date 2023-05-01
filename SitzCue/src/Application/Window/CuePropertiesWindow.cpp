#include "CuePropertiesWindow.h"

#include "Application/ImGuiDefaults.h"
#include "Project/CueList.h"

using namespace SitzCue;

void CuePropertiesWindow::OnUpdate(CueList& cueList, const std::vector<UUID>& selectedCues) {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::Begin("Cue Properties");

    if(selectedCues.size() <= 0) {
        ImGui::Text("No Cue Selected");
    } else if(selectedCues.size() > 1) {
        ImGui::Text("Please Select Just One Cue");
    } else {

        auto cue = cueList.GetCue(selectedCues[0]);

        if(cue != nullptr) {
            DrawCueHeader(*cue);
        }
    }

    ImGui::End();

}

void CuePropertiesWindow::DrawCueHeader(Cue& cue) {

    SITZCUE_PROFILE_FUNCTION();

    if(ImGui::TreeNodeEx("General", ImGuiTreeNodeFlags_DefaultOpen)) {

        if(!cue.CueName.empty()) {
            ImGui::Text("%s", cue.CueName.c_str());
            ImGui::NewLine();
        }

        ImGuiDefaults::DrawTextInput("Cue Name", cue.CueName);
        ImGuiDefaults::DrawOptionalFloat("Cue Number", cue.CueNumber);

        ImGui::TreePop();

    }

    if(cue.IdentifyCueType() == CueType::CueTypeSound) {

        SoundCue* soundCue = dynamic_cast<SoundCue*>(&cue);

        if(ImGui::TreeNodeEx("Sound", ImGuiTreeNodeFlags_DefaultOpen)) {

            ImGuiDefaults::FileDrop("Sound File Path", soundCue->SoundFilePath);

            ImGui::TreePop();
        }
 
    }
}