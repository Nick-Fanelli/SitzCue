#pragma once

#include "sitzcuepch.h"

#include "Cue.h"
#include "CueList.h"

#include "Utils/YAMLConversions.h"

namespace SitzCue {

    class Application;

    class Project {

        friend struct YAML::convert<Project>;

    public:
        static const inline std::string ProjectDirectoryExtension = ".sitzqprj";
        static const inline std::string ShowFileExtension = ".sitzqshow";

        static void CreateNewProject(Application* application);
        static void OpenProjectDialog(Application* application);

    public:
        Project(const std::filesystem::path& projectDirectoryPath) : m_ProjectDirectoryPath(projectDirectoryPath) {

            m_ProjectDirectoryPath = std::filesystem::canonical(m_ProjectDirectoryPath);

            m_ProjectName = m_ProjectDirectoryPath.stem();

            m_ShowFilePath = std::filesystem::path(m_ProjectDirectoryPath.string() + "/" +  m_ProjectDirectoryPath.stem().string() + std::string(Project::ShowFileExtension));
            m_AssetsDirectoryPath = std::filesystem::path(m_ProjectDirectoryPath.string() + "/" + "Assets");

            LoadProject();
        }

        const CueList& GetCueList() const { return m_CueList; }
        CueList& GetCueList() { return m_CueList; }

        const std::filesystem::path& GetProjectDirectoryPath() const { return m_ProjectDirectoryPath; }
        const std::filesystem::path& GetShowFilePath() const { return m_ShowFilePath; }
        const std::filesystem::path& GetAssetsDirectoryPath() const { return m_AssetsDirectoryPath; }

        const std::string& GetProjectName() const { return m_ProjectName; }

        void SaveProject();
        void LoadProject();

    private:
        std::filesystem::path m_ProjectDirectoryPath;
        std::filesystem::path m_ShowFilePath;
        std::filesystem::path m_AssetsDirectoryPath;

        std::string m_ProjectName;

        CueList m_CueList;

    };

}