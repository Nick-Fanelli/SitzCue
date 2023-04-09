#include "ImGuiDefaults.h"

#include "CommandSystem/CommandStack.h"

#include <imgui/misc/cpp/imgui_stdlib.h>

using namespace SitzCue;

void ImGuiDefaults::DrawTextInput(const std::string& label, Ref<std::string> data) {

    static std::string dataCache = std::string();
    dataCache = *data;

    ImGui::PushID(&*data);

    ImGui::Text("%s", label.c_str());
    ImGui::SameLine();
    ImGui::InputText("", &dataCache);

    if(ImGui::IsItemDeactivatedAfterEdit()) {
        CommandStack::ExecuteCommand<UpdateStringCommand>(data, *data, dataCache);
    }

    ImGui::PopID();
}

void ImGuiDefaults::DrawHiddenTextInput(Ref<std::string> data) {

    static std::string dataCache = std::string();
    dataCache = *data;

    ImGui::PushID(&*data);

    auto previousPadding = ImGui::GetStyle().FramePadding.y;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.0f, previousPadding});

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));

    ImGui::InputText("", &dataCache);

    if(ImGui::IsItemDeactivatedAfterEdit()) {
        CommandStack::ExecuteCommand<UpdateStringCommand>(data, *data, dataCache);
    }
 
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::PopID();
}

static std::string s_DrawCueNumberStringPlaceholder;

void ImGuiDefaults::DrawFloat(const std::string& label, Ref<float> data) {

    static float dataCache = 0.0f;
    dataCache = *data;

    ImGui::PushID(&*data);

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ImGuiDefaults::ColumnWidth * 3.0f);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn(); 

    if(!data.IsNull()) {
        ImGui::InputFloat("", &dataCache, 1.0f, 10.0f, "%g", ImGuiInputTextFlags_None);

        if(ImGui::IsItemDeactivatedAfterEdit()) {
            CommandStack::ExecuteCommand<UpdateFloatCommand>(data, *data, dataCache);
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove")) {
            data.DeleteData();
        }

    } else {

        s_DrawCueNumberStringPlaceholder.clear();

        ImGui::InputText("", &s_DrawCueNumberStringPlaceholder);
        if(ImGui::IsItemClicked()) {        
            CommandStack::ExecuteCommand<UpdateFloatCommand>(data, 0.0f, 0.0f);
        }   
    }

    ImGui::Columns(1);

    ImGui::PopID();

}

static std::string s_DrawCueNumberHiddenStringPlaceholder;

void ImGuiDefaults::DrawFloatHidden(Ref<float> data) {

    static float dataCache = 0.0f;
    dataCache = *data;

    ImGui::PushID(&*data);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});

    auto previousPadding = ImGui::GetStyle().FramePadding.y;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.0f, previousPadding});

    if(!data.IsNull()) {
        ImGui::InputFloat("", &dataCache, 0, 0, "%g", ImGuiInputTextFlags_None);

        if(ImGui::IsItemDeactivatedAfterEdit()) {
            CommandStack::ExecuteCommand<UpdateFloatCommand>(data, *data, dataCache);
        }

    } else {
        s_DrawCueNumberHiddenStringPlaceholder.clear();
        ImGui::InputText("", &s_DrawCueNumberHiddenStringPlaceholder);
        if(!s_DrawCueNumberHiddenStringPlaceholder.empty()) {
            CommandStack::ExecuteCommand<UpdateFloatCommand>(data, 0.0f, 0.0f);
        }
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::PopID();
}