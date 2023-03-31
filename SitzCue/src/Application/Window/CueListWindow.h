#pragma once

#include "sitzcuepch.h"

#include "Window.h"

#include "Project/Cue.h"

namespace SitzCue {

    class CueListWindow : public Window {

    public:
        CueListWindow();

        void OnUpdate() override;

        const std::vector<Cue*>& GetSelectedCues() const { return m_SelectedCues; }
        std::vector<Cue*>& GetSelectedCues() { return m_SelectedCues; }

    private:
        void HandleOnCueClick(Cue& cue);

        void DrawCue(Cue& cue);

        CueList m_CueList;
        std::vector<Cue*> m_SelectedCues;

    };


}