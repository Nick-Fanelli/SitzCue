#pragma once

#include "sitzcuepch.h"

#include "Window/Window.h"

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

        const WindowManager& GetWindowManager() const { return m_WindowManager; }
        WindowManager& GetWindowManager() { return m_WindowManager; }

    private:
        Application* m_Application;

        WindowManager m_WindowManager;

        void ApplyColorTheme();

    };

}