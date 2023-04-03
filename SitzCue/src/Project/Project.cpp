#include "Project.h"

#include <nfd.hpp>

#include "Application/Application.h"
#include "Utils/FileUtils.h"

using namespace SitzCue;

void Project::CreateNewProject(Application* application) {

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
    FileUtils::CreateDirectory(project.GetResourcesDirectoryPath());

    application->SetProject(project);
}

void Project::OpenProjectDialog(Application* application) {

    nfdchar_t* outPath = nullptr;
    nfdfilteritem_t filterItems[2] = { { "SitzQ Show File", "sitzqshow" } };   
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItems, 1, nullptr);

    if(result != NFD_OKAY)
        return;

    std::filesystem::path openPath(outPath);
    NFD_FreePath(outPath);

    std::filesystem::path projectPath =  std::filesystem::canonical(std::filesystem::path(openPath).remove_filename());

    Log::Info("Opening Project at: " + projectPath.string());

    Project project(projectPath);

    if(project.GetShowFilePath() != openPath) {
        Log::Warn("Show file and predicted show file do not match...\n\tStatus: Continuing");
    }

    application->SetProject(project);

}