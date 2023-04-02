#pragma once

#include "sitzcuepch.h"

namespace SitzCue {

    class MenuBar {

    public:
        MenuBar() = default;
        MenuBar(const MenuBar&) = delete;
    
        void OnUpdate();

    };

}