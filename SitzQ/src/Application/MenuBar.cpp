#include "MenuBar.h"

#include "Project/Project.h"

using namespace SitzQ;

// File
static inline void OnFileSaveProject(Application* application) { application->GetActiveProject()->SaveProject(); }
static inline void OnFileOpenProject(Application* application) { Project::OpenProjectDialog(application); }
static inline void OnFileNewProject(Application* application) { Project::CreateNewProject(application); }

static inline void HandleShortcuts(Application* application) {

    SITZCUE_PROFILE_FUNCTION();

    // Command
    if(PlatformDetection::IsNativeCommandKey()) {

        // SHIFT
        if(ImGui::IsKeyDown(ImGuiKey_ModShift)) {

            // Cmd+Shift+O (Open Project)
            if(ImGui::IsKeyPressed(ImGuiKey_O)) OnFileOpenProject(application);

            // Cmd+Shift+N (New Project)
            if(ImGui::IsKeyPressed(ImGuiKey_N)) OnFileNewProject(application);

            return; // Boot-Out because shift was pressed
        }

        // Cmd+S (Save Project)
        if(ImGui::IsKeyPressed(ImGuiKey_S)) OnFileSaveProject(application);

    }

}

static inline void FileMenu(Application* application) {

    SITZCUE_PROFILE_FUNCTION();

    if(ImGui::BeginMenu("File")) {

        if(ImGui::MenuItem("Save Project", PlatformDetection::ActivePlatform == PlatformDetection::PlatformMacOS ? "Cmd+S" : "Ctrl+S")) OnFileSaveProject(application);
        if(ImGui::MenuItem("Open Project", PlatformDetection::ActivePlatform == PlatformDetection::PlatformMacOS ? "Cmd+Shift+O" : "Ctrl+Shift+O")) OnFileOpenProject(application);
        if(ImGui::MenuItem("New Project", PlatformDetection::ActivePlatform == PlatformDetection::PlatformMacOS ? "Cmd+Shift+N" : "Ctrl+Shift+N")) OnFileNewProject(application);

        ImGui::EndMenu();
    }

}

void MenuBar::OnUpdate() {

    SITZCUE_PROFILE_FUNCTION();

    HandleShortcuts(m_ApplicationPtr);

    if(ImGui::BeginMenuBar()) {

        FileMenu(m_ApplicationPtr);

        ImGui::EndMenuBar();

    }
    
}