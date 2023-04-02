#include "ImGuiDefaults.h"

#include <imgui/misc/cpp/imgui_stdlib.h>

using namespace SitzCue;

void ImGuiDefaults::DrawTextInput(const std::string& label, std::string& data) {
    ImGui::PushID(label.c_str());

    ImGui::Text("%s", label.c_str());
    ImGui::SameLine();
    ImGui::InputText("", &data);

    ImGui::PopID();
}

void ImGuiDefaults::DrawHiddenTextInput(std::string& data) {
    ImGui::PushID(&data);

    auto previousPadding = ImGui::GetStyle().FramePadding.y;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.0f, previousPadding});

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));

    ImGui::InputText("", &data);

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::PopID();
}

void ImGuiDefaults::DrawFloat(const std::string& label, float& value, float step, float stepFast, const char* format) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ImGuiDefaults::ColumnWidth * 3.0f);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();
    ImGui::InputFloat("", &value, step, stepFast, format, ImGuiInputTextFlags_None);

    ImGui::Columns(1);

    ImGui::PopID();
}

void ImGuiDefaults::DrawFloatHidden(float& value, float step, float stepFast, const char* format) {
    ImGui::PushID(&value);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});

    auto previousPadding = ImGui::GetStyle().FramePadding.y;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.0f, previousPadding});

    ImGui::InputFloat("", &value, 0, 0, format, ImGuiInputTextFlags_None);

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::PopID();
}

static float s_DrawCueNumberFloatPlaceholder;
static std::string s_DrawCueNumberStringPlaceholder;

void ImGuiDefaults::DrawCueNumber(const std::string& label, CueNumber& cueNumber) {

    ImGui::PushID(&cueNumber);

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ImGuiDefaults::ColumnWidth * 3.0f);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn(); 
    
    if(cueNumber.IsAssigned()) {
        s_DrawCueNumberFloatPlaceholder = cueNumber;
        ImGui::InputFloat("", &s_DrawCueNumberFloatPlaceholder, 1.0f, 10.0f, "%g", ImGuiInputTextFlags_None);
        ImGui::SameLine();
        cueNumber = s_DrawCueNumberFloatPlaceholder;

        if(ImGui::Button("Remove")) {
            cueNumber.Unassign();
        }

    } else {        
        s_DrawCueNumberStringPlaceholder.clear();

        ImGui::InputText("", &s_DrawCueNumberStringPlaceholder);
        if(ImGui::IsItemClicked()) {        
            cueNumber = 0.0f;
        }   
    }

    ImGui::Columns(1);

    ImGui::PopID();

}

static float s_DrawCueNumberHiddenFloatPlaceholder;
static std::string s_DrawCueNumberHiddenStringPlaceholder;

void ImGuiDefaults::DrawCueNumberHidden(CueNumber& cueNumber) {
    ImGui::PushID(&cueNumber);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});

    auto previousPadding = ImGui::GetStyle().FramePadding.y;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.0f, previousPadding});

    if(cueNumber.IsAssigned()) {
        s_DrawCueNumberHiddenFloatPlaceholder = cueNumber;
        ImGui::InputFloat("", &s_DrawCueNumberHiddenFloatPlaceholder, 0, 0, "%g", ImGuiInputTextFlags_None);
        cueNumber = s_DrawCueNumberHiddenFloatPlaceholder;
    } else {
        s_DrawCueNumberHiddenStringPlaceholder.clear();
        ImGui::InputText("", &s_DrawCueNumberHiddenStringPlaceholder);
        if(!s_DrawCueNumberHiddenStringPlaceholder.empty()) {
            cueNumber = 0.0f;
        }
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::PopID();
}