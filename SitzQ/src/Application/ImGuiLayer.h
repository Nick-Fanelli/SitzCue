#pragma once

#include "sitzqpch.h"

#include "Scene/Scene.h"

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

        const SceneManager& GetSceneManager() const { return m_SceneManager; }
        SceneManager& GetSceneManager() { return m_SceneManager; }

        static ImFont* GetBoldFont() { return s_BoldFont; }

    private:
        static inline ImFont* s_BoldFont = nullptr;

        Application* m_Application;
        SceneManager m_SceneManager;

        void ApplyColorTheme();

    };

}