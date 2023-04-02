#pragma once

#include "sitzcuepch.h"

#include "Display.h"

namespace SitzCue {

    class Application {

    public:

        void StartApplication();

        void OnUpdate(float deltaTime);

        const Display& GetDisplay() const { return m_Display; }
        Display& GetDisplay() { return m_Display; }

    private:
        Display m_Display;

    };

}