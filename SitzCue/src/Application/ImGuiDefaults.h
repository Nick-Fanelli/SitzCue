#pragma once

#include "sitzcuepch.h"

#include "Project/Cue.h"

static constexpr float FloatMin = std::numeric_limits<float>::min();
static constexpr float FloatMax = std::numeric_limits<float>::max();

namespace SitzCue::ImGuiDefaults {

    static constexpr float ColumnWidth = 75.0f;

    void DrawTextInput(const std::string& label, Ref<std::string> data);
    void DrawHiddenTextInput(Ref<std::string> data);

    void DrawFloat(const std::string& label, float& value, float step = 1.0f, float stepFast = 10.0f, const char* format = "%g");
    void DrawFloatHidden(float& value, float step = 1.0f, float stepFast = 10.0f, const char* format = "%g");

    void DrawCueNumber(const std::string& label, CueNumber& cueNumber);
    void DrawCueNumberHidden(CueNumber& cueNumber);


}