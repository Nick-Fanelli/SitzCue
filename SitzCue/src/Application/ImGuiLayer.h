#pragma once

#include "sitzcuepch.h"

namespace SitzCue {

    class Application;

    class ImGuiLayer {

    public:
        ImGuiLayer() = default;
        ImGuiLayer(Application* application);

        void OnCreate();

        void Begin();
        void End();

        void OnDestroy();

    private:
        Application* m_Application;

        void ApplyColorTheme();

    };

}