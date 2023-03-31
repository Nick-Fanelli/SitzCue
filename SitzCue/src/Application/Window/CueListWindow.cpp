#include "CueListWindow.h"

#include "Application/ImGuiDefaults.h"

using namespace SitzCue;

static constexpr ImVec4 ActiveCueColor = { 0.0f, 74.0f / 255.0f, 204.0f / 255.0f, 1.0f };

CueListWindow::CueListWindow() {
    m_CueList = CueList();

    m_CueList.CreateCue(CueType::CueTypeEmpty, 1, "Some Cue");
    m_CueList.CreateCue(CueType::CueTypeEmpty, 2, "Some Cue");
    m_CueList.CreateCue(CueType::CueTypeEmpty, 3, "Some Cue");
    m_CueList.CreateCue(CueType::CueTypeEmpty, 4, "Some Cue");
}

void CueListWindow::HandleOnCueClick(Cue& cue) {
    static auto& io = ImGui::GetIO();

    if(!io.KeyMods)
            m_SelectedCues.clear();
        m_SelectedCues.push_back(&cue);
}

void CueListWindow::DrawCue(Cue& cue) {
    
    SITZCUE_PROFILE_FUNCTION();

    bool isSelected = std::find(m_SelectedCues.begin(), m_SelectedCues.end(), &cue) != m_SelectedCues.end();

    ImGui::TableNextColumn();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("%s", CueTypeStrings[(int) cue.CueType].c_str());

    ImGui::TableNextColumn();

    if(isSelected) 
        ImGuiDefaults::DrawFloatHidden(cue.CueNumber);
    else {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%g", cue.CueNumber);
    }
    
    if(ImGui::IsItemClicked()) 
        HandleOnCueClick(cue);

    ImGui::TableNextColumn();
    if(isSelected)
        ImGuiDefaults::DrawHiddenTextInput(cue.CueName);
    else {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", cue.CueName.c_str());
    }
    if(ImGui::IsItemClicked()) 
        HandleOnCueClick(cue);

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Header, ActiveCueColor);

    if(isSelected)
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ActiveCueColor);

    if(ImGui::Selectable("", isSelected, ImGuiSelectableFlags_SpanAllColumns)) {
        HandleOnCueClick(cue);
    }

    ImGui::PopStyleColor(isSelected ? 2 : 1);
}

static bool s_IsNewCueDropdownVisible = false;
static char s_SearchBuffer[128];

void CueListWindow::OnUpdate() {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::Begin("Cue List");

    ImGui::Text("Search...");
    ImGui::SameLine();
    ImGui::InputText("###search_buffer", s_SearchBuffer, sizeof(s_SearchBuffer));
    
    ImGui::SameLine();
    // if(ImGui::Button("+###add_cue")) {
    //     s_IsNewCueDropdownVisible = true;
    // }

    if(ImGui::BeginCombo("###add_cue", "+", ImGuiComboFlags_NoPreview)) {
        ImGui::EndCombo();
    } 

    // if (ImGui::BeginComboPreview())
    // {
    //     ImGui::Text("Hello World");
    //     ImGui::EndComboPreview();
    // }

    if(ImGui::BeginTable("CueTable", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg)) {

        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Number");
        ImGui::TableSetupColumn("Name");
        ImGui::TableHeadersRow();

        for(Cue& cue : m_CueList.GetCues()) {
            ImGui::TableNextRow();
            ImGui::PushID(&cue);
            DrawCue(cue);
            ImGui::PopID();
        }

        ImGui::EndTable();

    }

    ImGui::End();
}