#pragma once

#include "sitzcuepch.h"

namespace SitzCue {

    class Application;

    class StatusBar {

    public:
        StatusBar() = default;
        StatusBar(Application* applicationPtr) : m_ApplicationPtr(applicationPtr) {}

        void OnUpdate();

        static constexpr float StatusBarHeight = 100;
    
    private:
        Application* m_ApplicationPtr;
    
    };

}