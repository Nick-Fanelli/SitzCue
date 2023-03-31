#include "CuePropertiesWindow.h"

#include "Application/ImGuiDefaults.h"

using namespace SitzCue;

CuePropertiesWindow::CuePropertiesWindow(CueListWindow* cueListWindowPtr) {
    m_CueListWindowPtr = cueListWindowPtr;
}

void CuePropertiesWindow::OnUpdate() {

    std::vector<Cue*> selectedCues = m_CueListWindowPtr->GetSelectedCues();

    ImGui::Begin("Cue Properties");

    if(selectedCues.size() <= 0) {
        ImGui::Text("No Cue Selected");
    } else {

        ImGuiDefaults::DrawTextInput("Cue Name", selectedCues[0]->CueName);
        ImGuiDefaults::DrawFloat("Cue Number", selectedCues[0]->CueNumber);

    }

    ImGui::End();

}