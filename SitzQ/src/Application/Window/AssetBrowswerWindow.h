#pragma once

#include "sitzqpch.h"

#include "Application/Application.h"

#include "Runtime/Asset.h"

namespace SitzQ {

    class AssetBrowserWindow {
        
    public:
        AssetBrowserWindow(Application* applicationPtr);
        ~AssetBrowserWindow();

        void OnUpdate();

    private:
        void UpdateLocalAssetCache();

    private:
        Application* m_ApplicationPtr;

    };

}