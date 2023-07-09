#include "Window.h"

#include "Application/Application.h"

#include "Runtime/Audio.h"

using namespace SitzQ;

AssetBrowserWindow::AssetBrowserWindow(Application* applicationPtr) : m_ApplicationPtr(applicationPtr) {

}

AssetBrowserWindow::~AssetBrowserWindow() {
    
}

static inline void DrawAssetTreeNode(const std::filesystem::path& filepath) {
    if(ImGui::TreeNodeEx(filepath.stem().c_str(), ImGuiTreeNodeFlags_Leaf)) {

        ImGui::TreePop();
    }
}

static void DrawAssetAsAudioSource(const std::filesystem::path& filepath, Asset* asset) {
    if(ImGui::TreeNodeEx(filepath.stem().c_str(), ImGuiTreeNodeFlags_Leaf)) {
                
        ImGui::TreePop();
    }
}

void AssetBrowserWindow::UpdateLocalAssetCache() {

}

void AssetBrowserWindow::OnUpdate() {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::Begin("Asset Browser");

    if(ImGui::TreeNodeEx(m_ApplicationPtr->GetActiveProject()->GetAssetsDirectoryPath().stem().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {

        // for(auto& asset : AssetManager::GetAssetRegistry()) {

        //     // DrawAssetAsAudioSource(asset.first, asset.second);

            

        // }

        ImGui::TreePop();
    }

    ImGui::End();

}