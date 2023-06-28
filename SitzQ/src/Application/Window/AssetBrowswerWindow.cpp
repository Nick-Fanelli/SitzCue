#include "AssetBrowswerWindow.h"

#include "Runtime/Asset.h"

using namespace SitzQ;

void AssetBrowserWindow::OnUpdate() {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::Begin("Asset Browser");

    if(ImGui::TreeNodeEx(m_ApplicationPtr->GetActiveProject()->GetAssetsDirectoryPath().stem().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {

        for(auto& asset : AssetManager::GetAssetRegistry()) {

            if(ImGui::TreeNodeEx(asset.first.stem().c_str(), ImGuiTreeNodeFlags_Leaf)) {
                ImGui::TreePop();
            }

        }

        ImGui::TreePop();
    }

    ImGui::End();

}