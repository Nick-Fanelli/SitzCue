#pragma once

#include "sitzcuepch.h"

namespace SitzCue {

    class Window {

    public:
        Window() = default;

        virtual void OnUpdate();
    };

}