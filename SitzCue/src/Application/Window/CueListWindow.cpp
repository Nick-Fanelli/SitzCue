#include "CueListWindow.h"

#include "Application/ImGuiDefaults.h"
#include "Application/CommandSystem/CommandStack.h"

using namespace SitzCue;

static constexpr ImVec4 SelectedCueColor = { 0.0f, 74.0f / 255.0f, 204.0f / 255.0f, 1.0f };
static constexpr ImVec4 ActiveCueColor = { 0.0f, 74.0f / 255.0f, 204.0f / 255.0f, 1.0f };

static constexpr int EmptyCueTemplate = 0;

void CueListWindow::HandleOnCueClick(const std::vector<Cue*>& cueCache, UUID uuid) {

    SITZCUE_PROFILE_FUNCTION();

    static auto& io = ImGui::GetIO();

    if(io.KeyShift) {

        if(m_SelectedCues.size() <= 0) {
            
            m_SelectedCues.push_back(uuid);

        } else {

            UUID lastSelectedUUID = m_SelectedCues[m_SelectedCues.size() - 1];

            if(lastSelectedUUID == uuid) {
                auto it = std::find(m_SelectedCues.begin(), m_SelectedCues.end(), uuid);

                if(it != m_SelectedCues.end())
                    m_SelectedCues.erase(it);
                return;
            }

            int lastSelectedIndex = -1;
            int currentSelectedIndex = -1;

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
                auto it = std::find(m_SelectedCues.begin(), m_SelectedCues.end(), cueCache[i]->UUID);
                if(it == m_SelectedCues.end()) {
                    m_SelectedCues.push_back(cueCache[i]->UUID);
                }

            }
        }

    } else if(io.KeySuper) {
        // Platform Detection
        Log::Warn("Unimplemented");
    } else {
        m_SelectedCues.clear();
        m_SelectedCues.push_back(uuid);
    }
}

void CueListWindow::DrawCue(CueList& cueList, const std::vector<Cue*>& cueCache, int n) {
    
    SITZCUE_PROFILE_FUNCTION();

    // Getting Active Cue
    Cue& cue = *cueCache[n];

    bool isSelected = std::find(m_SelectedCues.begin(), m_SelectedCues.end(), cue.UUID) != m_SelectedCues.end();

    ImGui::TableNextColumn();
    ImGui::AlignTextToFramePadding();

    ImGui::TableNextColumn();

    if(isSelected) {
        ImGuiDefaults::DrawHiddenOptionalFloat(cue.CueNumber);
    }
    else {
        ImGui::AlignTextToFramePadding();
        if(cue.CueNumber.has_value())
            ImGui::Text("%g", (double) *cue.CueNumber);
        else
            ImGui::Text("");
    }
    
    ImGui::TableNextColumn();
    if(isSelected)
        ImGuiDefaults::DrawHiddenTextInput(cue.CueName);
    else {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", cue.CueName.c_str());
    }

    ImGui::SameLine();

    if(isSelected) {
        ImGui::PushStyleColor(ImGuiCol_Header, SelectedCueColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, SelectedCueColor);
    }

    // Element Targets
    if(ImGui::Selectable("", isSelected, ImGuiSelectableFlags_SpanAllColumns)) {
        HandleOnCueClick(cueCache, cue.UUID);
    }

    if(ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload("DND_CUE", (void*) &EmptyCueTemplate, sizeof(EmptyCueTemplate));
        if(cue.CueNumber.has_value()) {
            ImGui::Text("%g - %s", *cue.CueNumber, cue.CueName.c_str());
        } else if(cue.CueName.empty()) {
            ImGui::Text("Unnamed Cue");
        } else {
            ImGui::Text("%s", cue.CueName.c_str());
        }

        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget()) {

        if(ImGui::AcceptDragDropPayload("DND_CUE", ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect)) {

            ImGuiWindow* window = ImGui::GetCurrentWindow();

            const ImVec2 cursorPosition = ImGui::GetCursorPos();
            const ImVec2 startingPosition = ImVec2{ window->Pos.x, window->Pos.y + cursorPosition.y };

            window->DrawList->AddLine(startingPosition, ImVec2{ window->Size.x + 200.0f, startingPosition.y }, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 2.0f);

        }

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_CUE", ImGuiDragDropFlags_AcceptNoDrawDefaultRect)) {
            // Do something with the dropped payload
            Log::Info("Accept");
        }

        ImGui::EndDragDropTarget();
    }

    if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("Cue Right Click Menu");
    }

    // Right Click Menu
    if(ImGui::BeginPopup("Cue Right Click Menu")) {
        if(ImGui::MenuItem("Delete")) {
            CommandStack::ExecuteCommand(new DeleteCueCommand(cueList, cue.UUID));
        }   

        ImGui::EndPopup();
    }

    if(isSelected)
        ImGui::PopStyleColor(2);
}

static void DrawNewCueTemplateButtons() {
    SITZCUE_PROFILE_FUNCTION();

    static constexpr float cueTemplateButtonScale = 2.0f;

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.0f);
    ImGui::GetStyle().ScaleAllSizes(cueTemplateButtonScale);
    ImGui::Button("\uf49e");
    ImGui::GetStyle().ScaleAllSizes(1.0f / cueTemplateButtonScale);
    ImGui::PopStyleVar();

    if(ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload("DND_CUE", (void*) &EmptyCueTemplate, sizeof(EmptyCueTemplate));
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
        ImGui::SetDragDropPayload("DND_CUE", (void*) &EmptyCueTemplate, sizeof(EmptyCueTemplate));
        ImGui::Text("Empty Cue");
        ImGui::EndDragDropSource();
    }

    ImGui::PopStyleColor();
}

static bool s_IsNewCueDropdownVisible = false;

void CueListWindow::OnUpdate(CueList& cueList) {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::Begin("Cue List");

    if(ImGui::IsWindowFocused()) {

        if(PlatformDetection::IsNativeCommandKey() && ImGui::IsKeyPressed(ImGuiKey_Backspace, false)) {

            // FIXME: Sometimes crashes and wrong order sometimes lol

            if(m_SelectedCues.size() > 0) {

                auto* batchCommand = new BatchCommand();

                for(UUID cueUUID : m_SelectedCues) {
                    batchCommand->PushBackCommand(new DeleteCueCommand(cueList, cueUUID));
                }

                CommandStack::ExecuteCommand(batchCommand);

                m_SelectedCues.clear();
            }
        }

    }

    DrawNewCueTemplateButtons();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.0f, 2.0f});

    if(ImGui::BeginTable("CueTable", 3, ImGuiTableFlags_Resizable)) {

        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Number");
        ImGui::TableSetupColumn("Name");
        ImGui::TableHeadersRow();

        const auto& cache = cueList.GetCueCache();

        for(int n = 0; n < cache.size(); n++) {

            ImGui::TableNextRow();

            ImGui::PushID(cache[n]);
            DrawCue(cueList, cache, n);
            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    ImGui::BeginChild("CueDropSection", ImGui::GetContentRegionAvail());
    ImGui::EndChild();

    ImGui::PopStyleVar();

    ImGui::End();

    m_CuePropertiesWindow.OnUpdate(cueList, m_SelectedCues);
}