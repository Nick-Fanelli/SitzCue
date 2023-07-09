#pragma once

#include "sitzqpch.h"

namespace SitzQ {

    class Application;

    class Display {

    public:
        Display() = default;
        Display(const Display&) = delete;

        void CreateDisplay(); // Create the display
        void StartApplicationLoop(Application* application); // Start main loop

        /**
         * Get the window size as an ImVec2
        */
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