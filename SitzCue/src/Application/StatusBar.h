#pragma once

#include "sitzcuepch.h"

namespace SitzCue {

    class StatusBar {

    public:
        StatusBar() = default;

        void OnUpdate();

        static constexpr float StatusBarHeight = 100;
    
    };

}