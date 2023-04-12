#pragma once

#include "sitzcuepch.h"

#include "Project/Cue.h"

static constexpr float FloatMin = std::numeric_limits<float>::min();
static constexpr float FloatMax = std::numeric_limits<float>::max();

namespace SitzCue::ImGuiDefaults {

    static constexpr float ColumnWidth = 75.0f;

    void DrawTextInput(const std::string& label, std::string& data);
    void DrawHiddenTextInput(std::string& data);

    void DrawOptionalFloat(const std::string& label, std::optional<float>& data);
    void DrawHiddenOptionalFloat(std::optional<float>& data);


}