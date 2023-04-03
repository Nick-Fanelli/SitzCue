#include "CueListWindow.h"

#include "Application/ImGuiDefaults.h"

using namespace SitzCue;

static constexpr ImVec4 SelectedCueColor = { 0.0f, 74.0f / 255.0f, 204.0f / 255.0f, 1.0f };
static constexpr ImVec4 ActiveCueColor = { 0.0f, 74.0f / 255.0f, 204.0f / 255.0f, 1.0f };

// CueListWindow::CueListWindow() {
//     m_CueList.CreateCue("House Open (Pre-Show)", 1.0f);
//     m_CueList.CreateCue();
//     m_CueList.CreateCue();
//     m_CueList.CreateCue("Pre-Show Announcement", 2.0f);
//     m_CueList.CreateCue();
//     m_CueList.CreateCue("Show Mode", 3.0f);
//     m_CueList.CreateCue("House Open (Post-Show)", 4.0f);
// }

void CueListWindow::HandleOnCueClick(UUID uuid) {

    SITZCUE_PROFILE_FUNCTION();

    static auto& io = ImGui::GetIO();

    if(io.KeyShift) {

        UUID lastSelectedUUID = m_SelectedCues[m_SelectedCues.size() - 1];

        if(lastSelectedUUID == uuid) {
            auto it = std::find(m_SelectedCues.begin(), m_SelectedCues.end(), uuid);
            m_SelectedCues.erase(it);
            return;
        }

        int lastSelectedIndex = -1;
        int currentSelectedIndex = -1;

        const auto& cueCache = m_CueList.GetCueCache();

        // Find Indices of the Selections
        for(int i = 0; i < cueCache.size(); i++) {
            if(cueCache[i]->UUID == lastSelectedUUID) {
                lastSelectedIndex = i;
            }

            if(cueCache[i]->UUID == uuid) {
                currentSelectedIndex = i;
            }

            if(currentSelectedIndex != -1 && lastSelectedIndex != -1)
                break;
        }

        int firstIndex = (lastSelectedIndex < currentSelectedIndex) ? lastSelectedIndex : currentSelectedIndex;
        int lastIndex = (lastSelectedIndex > currentSelectedIndex) ? lastSelectedIndex : currentSelectedIndex;

        for(int i = firstIndex; i <= lastIndex; i++) {
            m_SelectedCues.push_back(cueCache[i]->UUID);
        }

        return;

    } else if(io.KeySuper) {
        // Platform Detection
        Log::Info("Hey");
    }

    m_SelectedCues.clear();
    m_SelectedCues.push_back(uuid);
}

void CueListWindow::DrawCue(const std::vector<Cue*>& cues, int n) {
    
    SITZCUE_PROFILE_FUNCTION();

    Cue& cue = *cues[n];

    bool isSelected = std::find(m_SelectedCues.begin(), m_SelectedCues.end(), cue.UUID) != m_SelectedCues.end();

    ImGui::TableNextColumn();
    ImGui::AlignTextToFramePadding();
    // ImGui::Text("%s", CueTypeStrings[(int) cue.CueType].c_str());

    ImGui::TableNextColumn();

    if(isSelected) {
        ImGuiDefaults::DrawCueNumberHidden(cue.CueNumber);
    }
    else {
        ImGui::AlignTextToFramePadding();
        if(cue.CueNumber.IsAssigned())
            ImGui::Text("%g", (double) cue.CueNumber);
        else
            ImGui::Text("");
    }
    
    // Element Targets
    if(ImGui::IsItemClicked()) 
        HandleOnCueClick(cue.UUID);
    // HandleOnCueDrag(cue);

    ImGui::TableNextColumn();
    if(isSelected)
        ImGuiDefaults::DrawHiddenTextInput(cue.CueName);
    else {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", cue.CueName.c_str());
    }

    // Element Targets
    if(ImGui::IsItemClicked()) 
        HandleOnCueClick(cue.UUID);
    // HandleOnCueDrag(cue);

    ImGui::SameLine();


    if(isSelected) {
        ImGui::PushStyleColor(ImGuiCol_Header, SelectedCueColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, SelectedCueColor);
    }

    // Element Targets
    if(ImGui::Selectable("", isSelected, ImGuiSelectableFlags_SpanAllColumns)) {
        HandleOnCueClick(cue.UUID);
    }

    // ImGui::PopStyleColor(isSelected ? 2 : 1);
    if(isSelected)
        ImGui::PopStyleColor(2);
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

    if(ImGui::BeginCombo("###add_cue", "+", ImGuiComboFlags_NoPreview)) {
        ImGui::EndCombo();
    } 

    if(ImGui::BeginTable("CueTable", 3, ImGuiTableFlags_Resizable)) {

        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Number");
        ImGui::TableSetupColumn("Name");
        ImGui::TableHeadersRow();

        const auto& cache = m_CueList.GetCueCache();

        for(int n = 0; n < cache.size(); n++) {

            ImGui::TableNextRow();

            ImGui::PushID(cache[n]);
            DrawCue(cache, n);
            ImGui::PopID();

        }

        ImGui::EndTable();

    }

    ImGui::End();

    // ImGui::ShowDemoWindow();
}