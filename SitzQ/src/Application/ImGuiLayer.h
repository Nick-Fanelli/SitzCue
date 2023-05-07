#pragma once

#include "sitzcuepch.h"

#include "Window/Window.h"

namespace SitzQ {

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

        static ImFont* GetBoldFont() { return s_BoldFont; }

    private:
        static inline ImFont* s_BoldFont = nullptr;

        Application* m_Application;
        WindowManager m_WindowManager;

        void ApplyColorTheme();

    };

}