#pragma once

#include "sitzqpch.h"

#include "Window.h"

#include "Project/Cue.h"

namespace SitzQ {

    class CuePropertiesWindow {

    public:
        CuePropertiesWindow() = default;

        void OnUpdate(CueList& cueList, const std::vector<UUID>& selectedCues);
        void DrawCueHeader(Cue& cue);

        // void DrawSoundCueHeader();

    };

}