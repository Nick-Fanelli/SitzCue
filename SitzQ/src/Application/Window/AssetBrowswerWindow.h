#pragma once

#include "sitzqpch.h"

#include "Application/Application.h"

namespace SitzQ {

    class AssetBrowserWindow {
        
    public:
        AssetBrowserWindow(Application* applicationPtr)
            : m_ApplicationPtr(applicationPtr) {}

        void OnUpdate();

    private:
        Application* m_ApplicationPtr;

    };

}