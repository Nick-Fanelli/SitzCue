#include "Project.h"

#include <nfd.hpp>

#include "Utils/FileUtils.h"

using namespace SitzCue;

void Project::CreateNewProject() {

    nfdchar_t* outPath = nullptr;
    nfdresult_t result = NFD_PickFolder(&outPath, nullptr);

    if(result != NFD_OKAY)
        return;

    std::filesystem::path directoryPath = std::filesystem::path(outPath);
    NFD_FreePath(outPath);

    if(!FileUtils::IsDirectoryEmpty(directoryPath)) {
        // TODO: Add Warning
        // FileUtils::DeleteAllDirectoryContents(projectPath);
    }

    std::filesystem::path projectPath = std::filesystem::path(directoryPath).replace_extension(Project::ProjectDirectoryExtension);
    FileUtils::MoveFile(directoryPath, projectPath);

    Project project(projectPath);
    
    FileUtils::CreateFile(project.GetShowFilePath());
    FileUtils::CreateFile(project.GetCueListFilePath());
    FileUtils::CreateDirectory(project.GetResourcesDirectoryPath());
}