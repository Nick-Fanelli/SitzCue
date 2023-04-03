#include "MenuBar.h"

#include "Project/Project.h"

using namespace SitzCue;

static inline void FileMenu(Application* application) {

    if(ImGui::BeginMenu("File")) {

        if(ImGui::MenuItem("Save Project")) {}
        
        if(ImGui::MenuItem("Open Project")) {}

        if(ImGui::MenuItem("New Project"))
            Project::CreateNewProject(application);

        ImGui::EndMenu();
    }

}

void MenuBar::OnUpdate() {

    SITZCUE_PROFILE_FUNCTION();

    if(ImGui::BeginMenuBar()) {

        FileMenu(m_ApplicationPtr);

        ImGui::EndMenuBar();

    }
    
}