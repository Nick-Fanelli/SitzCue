#include "ImGuiDefaults.h"

#include "CommandSystem/CommandStack.h"

#include <imgui/misc/cpp/imgui_stdlib.h>

using namespace SitzCue;

void ImGuiDefaults::DrawTextInput(const std::string& label, std::string& data) {

    SITZCUE_PROFILE_FUNCTION();

    static std::string dataCache = std::string();
    dataCache = data;

    ImGui::PushID(&data);

    ImGui::Text("%s", label.c_str());
    ImGui::SameLine();
    ImGui::InputText("", &dataCache);

    if(ImGui::IsItemDeactivatedAfterEdit()) {
        CommandStack::ExecuteCommand(new UpdateGenericDataCommand<std::string>{data, data, dataCache});
    }

    ImGui::PopID();
}

void ImGuiDefaults::DrawHiddenTextInput(std::string& data) {

    SITZCUE_PROFILE_FUNCTION();

    static std::string dataCache = std::string();
    dataCache = data;

    ImGui::PushID(&data);

    auto previousPadding = ImGui::GetStyle().FramePadding.y;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.0f, previousPadding});

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));

    ImGui::InputText("", &dataCache);

    if(ImGui::IsItemDeactivatedAfterEdit()) {
        CommandStack::ExecuteCommand(new UpdateGenericDataCommand<std::string>{data, data, dataCache});
    }
 
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::PopID();
}

void ImGuiDefaults::DrawOptionalFloat(const std::string& label, std::optional<float>& data) {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::PushID(&data);

    ImGui::Text("%s", label.c_str());
    ImGui::SameLine();

    if(data.has_value()) {
        static float dataCache = 0.0f;
        dataCache = *data;

        ImGui::InputFloat("", &dataCache, 1.0f, 10.0f, "%g");

        if(ImGui::IsItemDeactivatedAfterEdit()) {
            CommandStack::ExecuteCommand(new UpdateGenericDataCommand<std::optional<float>>{data, data, dataCache});
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove")) {
            CommandStack::ExecuteCommand(new UpdateGenericDataCommand<std::optional<float>>{data, data, std::nullopt});
        }

    } else {
        static std::string dataCache = "";
        dataCache.clear();

        ImGui::InputText("", &dataCache);

        if(ImGui::IsItemDeactivatedAfterEdit()) {

            float value = 0.0f;

            try {
                value = std::stof(dataCache);
            } catch(...) {}

            CommandStack::ExecuteCommand(new UpdateGenericDataCommand<std::optional<float>>{data, data, value});
        }
    }

    ImGui::PopID();

}


void ImGuiDefaults::DrawHiddenOptionalFloat(std::optional<float>& data) {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::PushID(&data);

    auto previousPadding = ImGui::GetStyle().FramePadding.y;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.0f, previousPadding});

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));

    static std::string dataCache = "";
    dataCache = data.has_value() ? std::to_string(*data) : "";
    dataCache.erase(dataCache.find_last_not_of('0') + 1, std::string::npos);
    if(dataCache.back() == '.')
        dataCache.pop_back();

    ImGui::InputText("", &dataCache, ImGuiInputTextFlags_CharsDecimal);

    if(ImGui::IsItemDeactivatedAfterEdit()) {

        if(dataCache.empty()) {
            CommandStack::ExecuteCommand(new UpdateGenericDataCommand<std::optional<float>>{data, data, std::nullopt});
        } else {
            float value = 0.0f;

            try {
                value = std::stof(dataCache);
            } catch(...) {}

            CommandStack::ExecuteCommand(new UpdateGenericDataCommand<std::optional<float>>{data, data, value});
        }
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::PopID();

}

