#pragma once

#include "sitzcuepch.h"

namespace SitzCue {

    class Display {

    public:
        Display() = default;
        Display(const Display&) = delete;

        void CreateDisplay();
        void StartApplicationLoop(const std::function<void(float)>& updateLoop);

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