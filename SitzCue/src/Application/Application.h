#pragma once

#include "sitzcuepch.h"

namespace SitzCue {

    class Application {

    public:
        GLFWwindow* m_Window;

        void StartApplication();

    private:
        int m_CurrentFps = 0;

        void CreateWindow();
        void LaunchApplicationLoop();
        void CleanUp();

    };

}