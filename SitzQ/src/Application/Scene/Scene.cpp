#include "Scene.h"

using namespace SitzQ;

void SceneManager::OnUpdate() {
    if(m_ActiveScene != nullptr)
        m_ActiveScene->OnUpdate();
}

void SceneManager::OnDestroy() {
    if(m_ActiveScene != nullptr) {
        m_ActiveScene->OnDestroy();
        delete m_ActiveScene;
    }
}