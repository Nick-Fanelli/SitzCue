#include "AssetBrowswerWindow.h"

using namespace SitzQ;

// File Extensions
static const std::vector<std::string> s_SupportedAudioFileExtensions = { ".wav", ".aiff", ".ogg", ".mp3" };

static inline void DrawFile(const std::filesystem::path& filepath) {

    static constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    ImGui::TreeNodeEx(filepath.filename().c_str(), flags);

    std::string lowercaseExtension = filepath.extension();
    std::transform(lowercaseExtension.begin(), lowercaseExtension.end(), lowercaseExtension.begin(), ::tolower);

    if(std::find(s_SupportedAudioFileExtensions.begin(), s_SupportedAudioFileExtensions.end(), lowercaseExtension) != s_SupportedAudioFileExtensions.end()) { // Sound File

        if(ImGui::BeginDragDropSource()) {

            static std::filesystem::path pathCache;
            pathCache = filepath;

            ImGui::Text("%s", filepath.filename().c_str());

            ImGui::SetDragDropPayload("DND_AUDIO_FILEPATH", &pathCache, sizeof(pathCache));
            ImGui::EndDragDropSource();

        }

    }
}

static void DrawDirectory(const std::filesystem::path& directory, bool isRoot = false) {

    if(ImGui::TreeNodeEx(directory.stem().c_str(), isRoot ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {

        for(const auto& file : std::filesystem::directory_iterator(directory)) {

            if(file.path().filename().string().front() == '.') { // Hidden files
                continue;
            }

            if(file.is_directory()) {
                DrawDirectory(file);
            } else {
                DrawFile(file.path());
            }

        }

        ImGui::TreePop();
    }

}

void AssetBrowserWindow::OnUpdate() {

    SITZCUE_PROFILE_FUNCTION();

    ImGui::Begin("Asset Browser");

    DrawDirectory(m_ApplicationPtr->GetActiveProject()->GetAssetsDirectoryPath(), true);


    // for(const auto& file : std::filesystem::directory_iterator(m_ApplicationPtr->GetActiveProject()->GetAssetsDirectoryPath())) {
    //     if(std::filesystem::is_regular_file(file.status())) {

    //         if(ImGui::TreeNode(file.path().filename().string().c_str())) {
    //             ImGui::TreePop();
    //         }

    //     }
    // }


    ImGui::End();

}