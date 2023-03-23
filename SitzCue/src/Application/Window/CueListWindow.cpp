#include "CueListWindow.h"

using namespace SitzCue;

CueListWindow::CueListWindow() {
    m_CueList = CueList();

    m_CueList.CreateCue(CueType::CueTypeEmpty, 1, "Some Cue");
    m_CueList.CreateCue(CueType::CueTypeEmpty, 2, "Some Cue");
    m_CueList.CreateCue(CueType::CueTypeEmpty, 3, "Some Cue");
    m_CueList.CreateCue(CueType::CueTypeEmpty, 4, "Some Cue");
}

void CueListWindow::DrawCue(Cue& cue) {
    ImGui::TableNextRow();

    ImGui::TableSetColumnIndex(0);
    ImGui::Text("%s", CueTypeStrings[(int) cue.CueType].c_str());

    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%g", cue.CueNumber);

    ImGui::TableSetColumnIndex(2);
    ImGui::Text("%s", cue.CueName.c_str());

    ImGui::SameLine();

    bool isSelected = std::find(m_SelectedCues.begin(), m_SelectedCues.end(), &cue) != m_SelectedCues.end();

    ImGui::PushID(cue.CueNumber);

    if(ImGui::Selectable("", isSelected, ImGuiSelectableFlags_SpanAllColumns)) {
        m_SelectedCues.clear();
        m_SelectedCues.push_back(&cue);
    }
    
    ImGui::PopID();
}

void CueListWindow::OnUpdate() {

    ImGui::Begin("Cue List");

    if(ImGui::BeginTable("CueTable", 3, ImGuiTableFlags_Resizable)) {

        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Number");
        ImGui::TableSetupColumn("Name");
        ImGui::TableHeadersRow();

        for(Cue& cue : m_CueList.GetCues()) {
            DrawCue(cue);
        }

        ImGui::EndTable();

    }

    ImGui::End();

    ImGui::ShowDemoWindow();
}