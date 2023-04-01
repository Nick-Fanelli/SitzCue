#pragma once

#include "sitzcuepch.h"

#include "Window.h"

#include "Project/Cue.h"

namespace SitzCue {

    class CueListWindow : public Window {

    public:
        CueListWindow();

        void OnUpdate() override;

        const std::vector<UUID>& GetSelectedCues() const { return m_SelectedCues; }
        std::vector<UUID>& GetSelectedCues() { return m_SelectedCues; }

        const CueList& GetCueList() const { return m_CueList; }
        CueList& GetCueList() { return m_CueList; }
 
    private:
        void HandleOnCueClick(UUID cue);

        void DrawCue(const std::vector<Cue*>& cues, int n);

        CueList m_CueList;
        std::vector<UUID> m_SelectedCues;

    };


}