#pragma once

#include "sitzqpch.h"

#include "CuePropertiesWindow.h"

#include "Application/Application.h"
#include "Project/Cue.h"

namespace SitzQ {

    class CueListWindow {

    public:
        CueListWindow() = default;

        void OnUpdate(CueList& cueList);

        const std::vector<UUID>& GetSelectedCues() const { return m_SelectedCues; }
        std::vector<UUID>& GetSelectedCues() { return m_SelectedCues; }

    private:
        void HandleOnCueClick(const std::vector<Cue*>& cueCache, UUID cue);
        void DrawCue(CueList& cueList, const std::vector<Cue*>& cues, int n);

    private:
        CuePropertiesWindow m_CuePropertiesWindow;

        std::vector<UUID> m_SelectedCues;

    };


}