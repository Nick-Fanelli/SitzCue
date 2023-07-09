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

static std::vector<float> s_Data;

void EditorScene::OnUpdate() {
    SITZCUE_PROFILE_FUNCTION();

    m_StatusBarPtr->OnUpdate();
    m_CueListWindowPtr->OnUpdate(m_SceneManagerPtr->GetApplicationPtr()->GetActiveProject()->GetCueList());
    m_AssetBrowserWindow->OnUpdate();

    ImGui::Begin("Music Controls");

    if(s_AudioSource) {

        ImGui::Text("Duration: %g seconds", s_AudioSource.Get().value()->GetSpecs().Duration.value());
        ImGui::Text("%.1f / %g", s_AudioSource.Get().value()->GetCurrentPlaybackPosition(), s_AudioSource.Get().value()->GetSpecs().Duration.value());

        if(ImGui::Button("Play")) {
            s_AudioSource.Get().value()->Play();
        }

        if(ImGui::Button("Pause")) {
            s_AudioSource.Get().value()->Pause();
        }

        if(ImGui::Button("Stop")) {
            s_AudioSource.Get().value()->Stop();
        }

        float position = (float) s_AudioSource.Get().value()->GetCurrentPlaybackPosition();
        float max = (float) s_AudioSource.Get().value()->GetSpecs().Duration.value();

        ImGui::SliderFloat("Position", &position, 0.0f, max, "%.1f");

        if(ImGui::IsItemEdited()) {
            s_AudioSource.Get().value()->SetCurrentPlaybackPosition(position);
        }
    }

    if(ImGui::Button("Erase!")) {
        AssetManager::ClearRegistry();
    }

    ImGui::End();
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