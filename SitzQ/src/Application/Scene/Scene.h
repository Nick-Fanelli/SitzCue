#pragma once

#include "sitzqpch.h"

#include "Application/StatusBar.h"
#include "Application/Window/Window.h"

namespace SitzQ {

    class Application;
    class SceneManager;

    // ===================================================================================================================
    // Abstract Scene
    // ===================================================================================================================
    class Scene {

    public:
        Scene(SceneManager* sceneManagerPtr) : m_SceneManagerPtr(sceneManagerPtr) {}
        virtual ~Scene() = default;

        virtual void OnCreate() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnDestroy() = 0;

    protected:
        SceneManager* m_SceneManagerPtr = nullptr;

    };

    // ===================================================================================================================
    // Lander Scene
    // ===================================================================================================================
    class LanderScene : public Scene {

    public:
        LanderScene(SceneManager* sceneManagerPtr) : Scene(sceneManagerPtr) {}

        void OnCreate() override;
        void OnUpdate() override;
        void OnDestroy() override;

    };

    // ===================================================================================================================
    // Editor Scene
    // ===================================================================================================================
    class EditorScene : public Scene {

    public:
        EditorScene(SceneManager* sceneManagerPtr) : Scene(sceneManagerPtr) {}

        void OnCreate() override;
        void OnUpdate() override;
        void OnDestroy() override;

    private:
        StatusBar* m_StatusBarPtr = nullptr;
        CueListWindow* m_CueListWindowPtr = nullptr;
        AssetBrowserWindow* m_AssetBrowserWindow = nullptr;

    };

     // ===================================================================================================================
    // Editor Scene
    // ===================================================================================================================
    class SceneManager {

    public:
        SceneManager() = default;
        SceneManager(Application* applicationPtr) : m_ApplicationPtr(applicationPtr) {}

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
        const Application* GetApplicationPtr() const { return m_ApplicationPtr; }

    private:
        Application* m_ApplicationPtr = nullptr;
        Scene* m_ActiveScene = nullptr;

    };

}