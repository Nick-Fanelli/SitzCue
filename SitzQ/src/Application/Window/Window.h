#pragma once

#include "sitzqpch.h"

#include "Application/StatusBar.h"

namespace SitzQ {

    // ===================================================================================================================
    // Window
    // ===================================================================================================================

    class Window {

    public:
        Window() = default;

        virtual void OnUpdate();
    };

    class Application;
    class WindowManager;
    class CueListWindow;
    class CuePropertiesWindow; 
    class AssetBrowserWindow;

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
        AssetBrowserWindow* m_AssetBrowserWindow = nullptr;

    };

    // ===================================================================================================================
    // Window Manager
    // ===================================================================================================================

    class WindowManager {

    public:

        WindowManager() = default;
        WindowManager(Application* applicationPtr) : m_ApplicationPtr(applicationPtr) {}

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

        Application* GetApplicationPtr() { return m_ApplicationPtr; }

    private:
        Application* m_ApplicationPtr = nullptr;
        Scene* m_ActiveScene = nullptr;

    };

}