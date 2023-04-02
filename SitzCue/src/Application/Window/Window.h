#pragma once

#include "sitzcuepch.h"

#include "Application/StatusBar.h"

namespace SitzCue {

    // ===================================================================================================================
    // Window
    // ===================================================================================================================

    class Window {

    public:
        Window() = default;

        virtual void OnUpdate();
    };

    class WindowManager;
    class CueListWindow;
    class CuePropertiesWindow; 

    // ===================================================================================================================
    // Scene
    // ===================================================================================================================

    class Scene {

    public:
        Scene(WindowManager* windowManagerPtr) : m_WindowManagerPtr(windowManagerPtr) {}

        virtual void OnCreate();
        virtual void OnUpdate();
        virtual void OnDestroy();

    protected:
        WindowManager* m_WindowManagerPtr;

    };

    // ===================================================================================================================
    // Lander Scene
    // ===================================================================================================================

    class LanderScene : public Scene {

    public:
        LanderScene(WindowManager* windowManagerPtr) : Scene(windowManagerPtr) {}

        void OnCreate() override;
        void OnUpdate() override;
        void OnDestroy() override;

    };

    // ===================================================================================================================
    // Editor Scene
    // ===================================================================================================================

    class EditorScene : public Scene {

    public:
        EditorScene(WindowManager* windowManagerPtr) : Scene(windowManagerPtr) {}

        void OnCreate() override;
        void OnUpdate() override;
        void OnDestroy() override;

    private:
        StatusBar* m_StatusBarPtr = nullptr;
        CueListWindow* m_CueListWindowPtr = nullptr;
        CuePropertiesWindow* m_CuePropertiesWindowPtr = nullptr;

    };

    // ===================================================================================================================
    // Window Manager
    // ===================================================================================================================

    class WindowManager {

    public:

        template<typename T>
        void SetScene() {
            if(m_ActiveScene != nullptr) {
                Scene* prevScene = m_ActiveScene;
                m_ActiveScene = nullptr;
                prevScene->OnDestroy();
                delete prevScene;
            }

            Scene* newScene = new T(this);
            newScene->OnCreate();
            m_ActiveScene = newScene;
        }

        void OnUpdate();
        void OnDestroy();

    private:

        Scene* m_ActiveScene = nullptr;

    };

}