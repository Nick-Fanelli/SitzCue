#pragma once

#include "sitzcuepch.h"

#include "Window.h"

#include "Application/Application.h"
#include "Project/Cue.h"

namespace SitzCue {

    class CueListWindow : public Window {

    public:
        CueListWindow() = delete;
        CueListWindow(Application* application) : m_ApplicationPtr(application), m_CueList(application->GetActiveProject()->GetCueList()) {}

        void OnUpdate() override;

        const std::vector<UUID>& GetSelectedCues() const { return m_SelectedCues; }
        std::vector<UUID>& GetSelectedCues() { return m_SelectedCues; }

        CueList& GetCueList() { return m_CueList; }
 
    private:
        void HandleOnCueClick(UUID cue);
        void DrawCue(const std::vector<Cue*>& cues, int n);

    private:
        Application* m_ApplicationPtr = nullptr;
        CueList& m_CueList;
        std::vector<UUID> m_SelectedCues;

    };


}