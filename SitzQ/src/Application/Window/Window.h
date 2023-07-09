#pragma once

#include "sitzqpch.h"

#include "Project/Cue.h"

namespace SitzQ {

    // ===================================================================================================================
    // Abstract Window
    // ===================================================================================================================
    class Window {

    public:
        Window() = default;

    };


    // ===================================================================================================================
    // Cue Properties Window
    // ===================================================================================================================
    class CuePropertiesWindow : public Window {

    public:
        CuePropertiesWindow() = default;

        void OnUpdate(CueList& cueList, const std::vector<UUID>& selectedCues);
        void DrawCueHeader(Cue& cue);

    };


    // ===================================================================================================================
    // Cue List Window
    // ===================================================================================================================
    class CueListWindow : public Window {

    public:
        CueListWindow() = default;

        void OnUpdate(CueList& cueList);

        const std::vector<UUID>& GetSelectedCues() const { return m_SelectedCues; }
        std::vector<UUID>& GetSelectedCues() { return m_SelectedCues; }

    private:
        void HandleOnCueClick(const std::vector<Cue*>& cueCache, UUID cue);
        void DrawCue(CueList& cueList, const std::vector<Cue*>& cues, int n);
        void DrawNewCueTemplateButtons(CueList& cueList);

    private:
        CuePropertiesWindow m_CuePropertiesWindow;

        std::vector<UUID> m_SelectedCues;
    };

    // ===================================================================================================================
    // Asset Browser Window
    // ===================================================================================================================
    class Application;
    class AssetBrowserWindow {
        
    public:
        AssetBrowserWindow(Application* applicationPtr);
        ~AssetBrowserWindow();

        void OnUpdate();

    private:
        void UpdateLocalAssetCache();

    private:
        Application* m_ApplicationPtr;

    };

}