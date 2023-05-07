#pragma once

#include "sitzqpch.h"

#include "Application/Application.h"

namespace SitzQ {

    class MenuBar {

    public:
        MenuBar() = default;
        MenuBar(Application* applicationPtr) : m_ApplicationPtr(applicationPtr) {}
        MenuBar(const MenuBar&) = delete;
    
        void OnUpdate();
    
    private:
        Application* m_ApplicationPtr;

    };

}