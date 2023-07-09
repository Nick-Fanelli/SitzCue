#include "Scene.h"

#include "Application/Application.h"

#include "Runtime/RuntimeEngine.h"
#include "Runtime/Asset.h"
#include "Runtime/Audio.h"

using namespace SitzQ;

// ===================================================================================================================
// Editor Scene
// ===================================================================================================================

static AssetHandle<AudioSource> s_AudioSource; 

void EditorScene::OnCreate() {
    SITZCUE_PROFILE_FUNCTION();

    if(m_SceneManagerPtr->GetApplicationPtr()->GetActiveProject() == nullptr) {
        Log::Error("No Project Found");
        m_SceneManagerPtr->SetScene<LanderScene>();
        return;
    }

    RuntimeEngine::InitializeAsync();    
    AssetManager::InitializeAsync();

    m_StatusBarPtr = new StatusBar(m_SceneManagerPtr->GetApplicationPtr());
    m_CueListWindowPtr = new CueListWindow();
    m_AssetBrowserWindow = new AssetBrowserWindow(m_SceneManagerPtr->GetApplicationPtr());

    s_AudioSource = AssetManager::CreateAsset<AudioSource>("/Users/nickfanelli/Desktop/Example Project.sitzqprj/Assets/Kassi Ashton - Drive You Out Of My Mind (Official Audio).mp3");
}

void EditorScene::OnUpdate() {
    SITZCUE_PROFILE_FUNCTION();

    m_StatusBarPtr->OnUpdate();
    m_CueListWindowPtr->OnUpdate(m_SceneManagerPtr->GetApplicationPtr()->GetActiveProject()->GetCueList());
    m_AssetBrowserWindow->OnUpdate();
}

void EditorScene::OnDestroy() {
    SITZCUE_PROFILE_FUNCTION();

    Log::Info("Destroying the editor scene...");

    delete m_StatusBarPtr;
    delete m_CueListWindowPtr;
    delete m_AssetBrowserWindow;

    RuntimeEngine::Terminate();
    AssetManager::Terminate();
}