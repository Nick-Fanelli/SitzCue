#pragma once

#include "sitzcuepch.h"

#include "Project/Cue.h"

static constexpr float FloatMin = std::numeric_limits<float>::min();
static constexpr float FloatMax = std::numeric_limits<float>::max();

namespace SitzQ::ImGuiDefaults {

    static constexpr ImVec4 AccentColor = { 0.0f, 74.0f / 255.0f, 204.0f / 255.0f, 1.0f };

    static constexpr float ColumnWidth = 75.0f;

    void DrawTextInput(const std::string& label, std::string& data);
    void DrawHiddenTextInput(std::string& data);

    void DrawOptionalFloat(const std::string& label, std::optional<float>& data);
    void DrawHiddenOptionalFloat(std::optional<float>& data);

    void FileDrop(const std::string& label, std::filesystem::path& filepath);

    bool DrawPropertiesHeader(const std::string& label);

}