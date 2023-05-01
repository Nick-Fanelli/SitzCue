#include "Project.h"

#include <nfd.hpp>

#include <yaml-cpp/yaml.h>

#include "Application/Application.h"
#include "Utils/FileUtils.h"
#include "Utils/YAMLConversions.h"

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

    Project* project = new Project(projectPath);

    FileUtils::CreateFile(project->GetShowFilePath());
    FileUtils::CreateDirectory(project->GetAssetsDirectoryPath());

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

    Project* project = new Project(projectPath);

    if(project->GetShowFilePath() != openPath) {
        Log::Warn("Show file and predicted show file do not match...\n\tStatus: Continuing");
    }

    application->SetProject(project);

}

void Project::SaveProject() {

    Log::Info("Saving Project"); // Trace

    YAML::Node node = YAML::convert<Project>::encode(*this);

    std::ofstream file(m_ShowFilePath);

    if(file.is_open()) {
        file << node;
        file.close();
    }

}   

void Project::LoadProject() {

    std::ifstream in(m_ShowFilePath);
    std::stringstream stream;
    stream << in.rdbuf();
    in.close();

    YAML::Node node = YAML::Load(stream.str());
    YAML::convert<Project>::decode(node, *this);

    // this->m_CueList.CreateCue(CueType::CueTypeSound);
    // this->m_CueList.CreateCue(CueType::CueTypeEmpty);
}