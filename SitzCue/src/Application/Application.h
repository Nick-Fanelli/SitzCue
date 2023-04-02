#pragma once

#include "sitzcuepch.h"

namespace SitzCue {

    class Application {

    public:

        void StartApplication();

        void GetImGuiSize(ImVec2* outVec2);

        GLFWwindow* GetWindowPtr() { return m_Window; }

    private:
        GLFWwindow* m_Window;

        int m_CurrentFps = 0;

        void CreateWindow();
        void LaunchApplicationLoop();
        void CleanUp();

    };

}