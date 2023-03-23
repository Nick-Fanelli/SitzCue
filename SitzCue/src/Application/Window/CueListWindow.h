#pragma once

#include "sitzcuepch.h"

#include "Window.h"

#include "Project/Cue.h"

namespace SitzCue {

    class CueListWindow : public Window {

    public:
        CueListWindow();

        void OnUpdate() override;

    private:
        void DrawCue(Cue& cue);

        CueList m_CueList;
        std::vector<Cue*> m_SelectedCues;

    };


}