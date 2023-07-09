#include "Window.h"

#include "Application/Application.h"
#include "Application/ImGuiDefaults.h"
#include "Application/CommandSystem/CommandStack.h"

using namespace SitzQ;

static constexpr ImVec4 SelectedCueColor = { 0.0f, 74.0f / 255.0f, 204.0f / 255.0f, 1.0f };
static constexpr ImVec4 ActiveCueColor = { 0.0f, 74.0f / 255.0f, 204.0f / 255.0f, 1.0f };
static constexpr ImVec4 ErrorColor { 1.0f, 0.3f, 0.3f, 1.0f };

static constexpr int EmptyCueTemplate = 0;
static constexpr int SoundCueTemplate = 1;

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

static inline CueType ConvertCueTemplateIntToCueType(int templateInt) {
    switch(templateInt) {

    case SoundCueTemplate:
        return CueType::CueTypeSound;
    case EmptyCueTemplate:
    default:
        return CueType::CueTypeEmpty;

    }
}

static inline void HandleCueTemplateCueExecution(CueList& cueList, int templateInt, std::optional<UUID> followCue = {}) {

    CueType cueType = ConvertCueTemplateIntToCueType(templateInt);

    if(followCue.has_value())
        CommandStack::ExecuteCommand(new CreateNewCueCommand(cueList, cueType, followCue.value()));
    else {
        if(cueList.CueListOrderSize() > 0) {
            CommandStack::ExecuteCommand(new CreateNewCueCommand(cueList, cueType, cueList.LastUUID()));
        } else {
            CommandStack::ExecuteCommand(new CreateNewCueCommand(cueList, cueType));
        }
    }
}   

void CueListWindow::DrawCue(CueList& cueList, const std::vector<Cue*>& cueCache, int n) {
    
    SITZCUE_PROFILE_FUNCTION();

    // Getting Active Cue
    Cue& cue = *cueCache[n];

    bool isCueValid = cue.IsValid();

    bool isSelected = std::find(m_SelectedCues.begin(), m_SelectedCues.end(), cue.UUID) != m_SelectedCues.end();

    if(!isCueValid && !isSelected) {
        ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.2f, 0.2f, 1.0f });
    }

    ImGui::TableNextColumn();
    ImGui::AlignTextToFramePadding();

    // Type
    if(dynamic_cast<SoundCue*>(cueCache[n])) {
        ImGui::Text("\uf028"); // Sound
    } else {
        ImGui::Text("\uf49e"); // Empty
    }

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
        ImGui::PushStyleColor(ImGuiCol_Header, isCueValid ? SelectedCueColor : ErrorColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, isCueValid ? SelectedCueColor : ErrorColor);
    }

    // Element Targets
    if(ImGui::Selectable("", isSelected, ImGuiSelectableFlags_SpanAllColumns)) {
        HandleOnCueClick(cueCache, cue.UUID);
    }

    if (ImGui::BeginDragDropTarget()) {

        if(ImGui::AcceptDragDropPayload(ImGuiDefaults::DragDropType::Cue, ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect)
            || ImGui::AcceptDragDropPayload(ImGuiDefaults::DragDropType::CueTemplate, ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect)) {

            const ImVec2 cursorPosition = ImGui::GetCursorPos();
            const ImVec2 startingPosition = ImVec2{ ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + cursorPosition.y };

            ImGuiWindow* window = ImGui::GetCurrentWindow();

            window->DrawList->AddLine(startingPosition, ImVec2{ startingPosition.x + ImGui::GetWindowWidth(), startingPosition.y }, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 2.5f);

        }

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ImGuiDefaults::DragDropType::Cue, ImGuiDragDropFlags_AcceptNoDrawDefaultRect)) {

            UUID payloadUUID = *static_cast<UUID*>(payload->Data);

            auto payloadCue = cueList.GetCue(payloadUUID);

            CommandStack::ExecuteCommand(new MoveCueCommand(cueList, payloadUUID, cue.UUID));

        }

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ImGuiDefaults::DragDropType::CueTemplate, ImGuiDragDropFlags_AcceptNoDrawDefaultRect)) {
            HandleCueTemplateCueExecution(cueList, *static_cast<int*>(payload->Data), cue.UUID);
        }

        ImGui::EndDragDropTarget();
    }

    if(ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload(ImGuiDefaults::DragDropType::Cue, (void*) &cue.UUID, sizeof(UUID));

        if(cue.CueNumber.has_value()) {
            ImGui::Text("%g - %s", *cue.CueNumber, cue.CueName.c_str());
        } else if(cue.CueName.empty()) {
            ImGui::Text("Unnamed Cue");
        } else {
            ImGui::Text("%s", cue.CueName.c_str());
        }

        ImGui::EndDragDropSource();
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
    else if(!isCueValid)
        ImGui::PopStyleColor();
}

static void DrawTemplateButton(const std::string& buttonLabel, const std::string& fullName, void* payload, CueType cueType, CueList& cueList) {

    SITZCUE_PROFILE_FUNCTION();

    static constexpr float cueTemplateButtonScale = 2.0f;

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.0f);
    ImGui::GetStyle().ScaleAllSizes(cueTemplateButtonScale);
    ImGui::Button(buttonLabel.c_str());
    ImGui::GetStyle().ScaleAllSizes(1.0f / cueTemplateButtonScale);
    ImGui::PopStyleVar();

    if(ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload(ImGuiDefaults::DragDropType::CueTemplate, payload, sizeof(int));
        ImGui::Text("%s", fullName.c_str());
        ImGui::EndDragDropSource();
    }
    
    if(ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
        if(cueList.CueListOrderSize() <= 0)
            CommandStack::ExecuteCommand(new CreateNewCueCommand(cueList, cueType));
        else {
            CommandStack::ExecuteCommand(new CreateNewCueCommand(cueList, cueType, cueList.LastUUID()));
        }
    }

    ImGui::PopStyleColor();

    ImGui::SameLine();

}

void CueListWindow::DrawNewCueTemplateButtons(CueList& cueList) {
    SITZCUE_PROFILE_FUNCTION();

    static constexpr float cueTemplateButtonScale = 2.0f;

    DrawTemplateButton("\uf49e", "Empty Cue", (void*) &EmptyCueTemplate, CueType::CueTypeEmpty, cueList);
    DrawTemplateButton("\uf028", "Sound Cue", (void*) &SoundCueTemplate, CueType::CueTypeSound, cueList);

    ImGui::NewLine();
}

static bool s_IsNewCueDropdownVisible = false;

void CueListWindow::OnUpdate(CueList& cueList) {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::Begin("Cue List");

    if(ImGui::IsWindowFocused()) {

        if(PlatformDetection::IsNativeCommandKey() && ImGui::IsKeyPressed(ImGuiKey_Backspace, false)) {

            // FIXME
            // TODO: Sometimes crashes and wrong order sometimes lol

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

    DrawNewCueTemplateButtons(cueList);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.0f, 2.0f});

    if(ImGui::BeginTable("CueTable", 4, ImGuiTableFlags_Resizable)) {

        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Number");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Action");
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

    ImVec2 overflowSize{ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 10.0f };

    ImGui::BeginChild("DragDropOverflow", overflowSize, false);
    ImGui::EndChild();

    if(ImGui::BeginDragDropTarget()) {

        if(ImGui::AcceptDragDropPayload(ImGuiDefaults::DragDropType::CueTemplate, ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect)) {

            const ImVec2 cursorPosition = ImGui::GetCursorPos();
            const ImVec2 startingPosition = ImVec2{ ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + cursorPosition.y - overflowSize.y - 5.0f };

            ImGuiWindow* window = ImGui::GetCurrentWindow();

            window->DrawList->AddLine(startingPosition, ImVec2{ startingPosition.x + ImGui::GetWindowWidth(), startingPosition.y }, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 2.5f);

        }

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ImGuiDefaults::DragDropType::CueTemplate, ImGuiDragDropFlags_AcceptNoDrawDefaultRect)) {
            HandleCueTemplateCueExecution(cueList, *static_cast<int*>(payload->Data));
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::PopStyleVar();

    ImGui::End();

    m_CuePropertiesWindow.OnUpdate(cueList, m_SelectedCues);
}