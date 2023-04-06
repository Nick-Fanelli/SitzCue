#include "CueListWindow.h"

#include "Application/ImGuiDefaults.h"

using namespace SitzCue;

static constexpr ImVec4 SelectedCueColor = { 0.0f, 74.0f / 255.0f, 204.0f / 255.0f, 1.0f };
static constexpr ImVec4 ActiveCueColor = { 0.0f, 74.0f / 255.0f, 204.0f / 255.0f, 1.0f };

static constexpr int EmptyCueTemplate = 0;

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
    // if(ImGui::IsItemClicked()) 
    //     HandleOnCueClick(cue.UUID);
    // HandleOnCueDrag(cue);

    ImGui::TableNextColumn();
    if(isSelected)
        ImGuiDefaults::DrawHiddenTextInput(cue.CueName);
    else {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", cue.CueName.c_str());
    }

    // Element Targets
    // if(ImGui::IsItemClicked()) 
    //     HandleOnCueClick(cue.UUID);

    ImGui::SameLine();

    if(isSelected) {
        ImGui::PushStyleColor(ImGuiCol_Header, SelectedCueColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, SelectedCueColor);
    }

    // Element Targets
    if(ImGui::Selectable("", isSelected, ImGuiSelectableFlags_SpanAllColumns)) {
        HandleOnCueClick(cue.UUID);
    }

    if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("Cue Right Click Menu");
    }

    // Right Click Menu
    if(ImGui::BeginPopup("Cue Right Click Menu")) {
        if(ImGui::MenuItem("Delete")) {
            // TODO: Delete Cue
            // EditorActions::DeleteCueAction::Register(m_CueList, cue);
        }   

        ImGui::EndPopup();
    }

    if(isSelected)
        ImGui::PopStyleColor(2);
}

static bool s_IsNewCueDropdownVisible = false;

void CueListWindow::OnUpdate() {

    SITZCUE_PROFILE_FUNCTION();

    static constexpr float cueTemplateButtonScale = 2.0f;

    ImGui::Begin("Cue List");

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.0f);
    ImGui::GetStyle().ScaleAllSizes(cueTemplateButtonScale);
    ImGui::Button("\uf49e");
    ImGui::GetStyle().ScaleAllSizes(1.0f / cueTemplateButtonScale);
    ImGui::PopStyleVar();

    if(ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload("DND_CUE_TYPE", (void*) &EmptyCueTemplate, sizeof(EmptyCueTemplate));
        ImGui::Text("Empty Cue");
        ImGui::EndDragDropSource();
    }

    ImGui::SameLine();
    
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.0f);
    ImGui::GetStyle().ScaleAllSizes(cueTemplateButtonScale);
    ImGui::Button("\uf028");
    ImGui::GetStyle().ScaleAllSizes(1.0f / cueTemplateButtonScale);
    ImGui::PopStyleVar();

    if(ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload("DND_CUE_TYPE", (void*) &EmptyCueTemplate, sizeof(EmptyCueTemplate));
        ImGui::Text("Empty Cue");
        ImGui::EndDragDropSource();
    }

    ImGui::PopStyleColor();

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

    ImGui::BeginChild("CueDropSection", ImGui::GetContentRegionAvail());
    ImGui::EndChild();

    if(ImGui::BeginDragDropTarget()) {

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_CUE_TYPE")) {
            int cueTemplate = *(int*) payload->Data;

            switch(cueTemplate) {

            case EmptyCueTemplate:
                // TODO Create Cue
                // EditorActions::CreateCueAction::Register(m_CueList);
                break;
            default:
                break;

            }

        }


        ImGui::EndDragDropTarget();
    }

    ImGui::End();

    // ImGui::ShowDemoWindow();
}