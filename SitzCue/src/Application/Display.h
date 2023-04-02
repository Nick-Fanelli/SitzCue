#pragma once

#include "sitzcuepch.h"

namespace SitzCue {

    class Application;

    class Display {

    public:
        Display() = default;
        Display(const Display&) = delete;

        void CreateDisplay();
        void StartApplicationLoop(Application* application);

        void GetImGuiSize(ImVec2* outVec2);

        const GLFWwindow* GetWindowPtr() const { return m_Window; }
        GLFWwindow* GetWindowPtr() { return m_Window; }

    private:
        void CleanUp();

    private:
        GLFWwindow* m_Window;
        int m_CurrentFps = 0;

    };

}