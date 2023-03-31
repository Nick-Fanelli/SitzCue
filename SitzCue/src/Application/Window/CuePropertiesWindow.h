#pragma once

#include "sitzcuepch.h"

#include "Window.h"
#include "CueListWindow.h"

namespace SitzCue {

    class CuePropertiesWindow : public Window {

    public:
        CuePropertiesWindow(CueListWindow* cueListWindowPtr);

        void OnUpdate() override;

    private:
        CueListWindow* m_CueListWindowPtr;

    };

}