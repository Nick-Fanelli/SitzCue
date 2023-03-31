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