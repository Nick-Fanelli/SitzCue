#pragma once

#include "sitzcuepch.h"

#include "Application/StatusBar.h"

namespace SitzCue {

    class Window {

    public:
        Window() = default;

        virtual void OnUpdate();
    };

    class CueListWindow;
    class CuePropertiesWindow; 

    class WindowManager {

    public:
        
        void OnCreate();
        void OnUpdate();
        void OnDestroy();

    private:

        StatusBar* m_StatusBarPtr = nullptr;
        CueListWindow* m_CueListWindowPtr = nullptr;
        CuePropertiesWindow* m_CuePropertiesWindowPtr = nullptr;

    };

}