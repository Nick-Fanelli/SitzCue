#pragma once

#include "sitzcuepch.h"

#include "Display.h"
#include "ImGuiLayer.h"

#include "Project/Project.h"

namespace SitzCue {

    class Application {

    public:

        void StartApplication();

        void OnCreate();
        void OnUpdate(float deltaTime);
        void OnDestroy();

        void SetProject(Project* project);

        const Display& GetDisplay() const { return m_Display; }
        Display& GetDisplay() { return m_Display; }

        const Project* GetActiveProject() const { return m_ActiveProject; }
        Project* GetActiveProject() { return m_ActiveProject; }

    private:
        Display m_Display;
        ImGuiLayer m_ImGuiLayer;

        Project* m_ActiveProject;

    };

}