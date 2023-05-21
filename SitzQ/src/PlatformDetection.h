#pragma once

#include <stdlib.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include "Utils/FileUtils.h"

#ifdef _WIN32
    #ifdef _WIN64
        #define SITZCUE_PLATFORM_WINDOWS
    #else  
        #error "x86 Builds are Not Supported..."
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>

    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS Simulator Not Supported"
    #elif TARGET_OS_IPHONE == 1
        #define SITZCUE_PLATFORM_IOS
        #error "IOS is not supported"
    #elif TARGET_OS_MAC == 1
        #define SITZCUE_PLATFORM_MACOS
    #else
        #error "Unknown Apple Platform Not Supported"
    #endif
#elif defined(__ANDROID__)
    #define SITZCUE_PLATFORM_ANDROID
    #error "Android Ist Not Supported"
#elif defined(__linux__)
    #define SITZCUE_PLATFORM_LINUX
#else
    #error "Unknown Platform is Not Supported"
#endif

namespace SitzQ::PlatformDetection {

    enum Platform {
        PlatformMacOS, PlatformWindows, PlatformLinux
    };

    // Platform Specific

#ifdef SITZCUE_PLATFORM_WINDOWS
    
    inline Platform ActivePlatform = Platform::PlatformWindows;

    inline const std::filesystem::path ApplicationCacheDirectoryPath = "%APPDATA%\\Local\\SitzQ";
    inline const std::filesystem::path ApplicationSettingsDirectoryPath = "%APPDATA%\\Roaming\\SitzQ";

    inline bool IsNativeCommandKey() {
        static const auto& io = ImGui::GetIO();
        return io.KeyCtrl;
    }

#elif defined(SITZCUE_PLATFORM_MACOS)
    
    inline Platform ActivePlatform = Platform::PlatformMacOS;

    inline const char* homeDir = getenv("HOME");

    inline const std::filesystem::path ApplicationCacheDirectoryPath = std::string(homeDir) + std::string("/Library/Caches/SitzQ");
    inline const std::filesystem::path ApplicationSettingsDirectoryPath = std::string(homeDir) + std::string("/Library/Preferences/SitzQ");

    inline bool IsNativeCommandKey() {
        static const auto& io = ImGui::GetIO();
        return io.KeySuper;
    }

#elif defined(SITZCUE_PLATFORM_LINUX)
    
    inline Platform ActivePlatform = Platform::PlatformLinux;

    inline const std::filesystem::path ApplicationCacheDirectoryPath = "~/.cache/SitzQ";
    inline const std::filesystem::path ApplicationSettingsDirectoryPath = "~/.config/SitzQ";

    inline bool IsNativeCommandKey() {
        static const auto& io = ImGui::GetIO();
        return io.KeyCtrl;
    }
#endif

    // Universal

    inline const std::filesystem::path& GetSettingsFilePath() {
        static const std::filesystem::path settingsFilePath = std::filesystem::path(std::string(ApplicationSettingsDirectoryPath) + "/settings.yaml");
        static bool isCreated = false;

        if(!isCreated && !FileUtils::Exists(settingsFilePath))
            FileUtils::CreateFile(settingsFilePath);

        isCreated = true;
        return settingsFilePath;
    }

    inline const std::filesystem::path& GetIniFilePath() {
        static const std::filesystem::path iniFilePath = std::filesystem::path(std::string(ApplicationSettingsDirectoryPath) + "/application-state.ini");
        static bool isCreated = false;

        if(!isCreated && !FileUtils::Exists(iniFilePath))
            FileUtils::CreateFile(iniFilePath);

        isCreated = true;
        return iniFilePath;
    }

    inline const std::filesystem::path& GetApplicationCacheFilePath() {
        static const std::filesystem::path applicationCacheFilePath = std::filesystem::path(std::string(ApplicationCacheDirectoryPath) + "/application-cache.yaml");
        static bool isCreated = false;

        if(!isCreated && !FileUtils::Exists(applicationCacheFilePath))
            FileUtils::CreateFile(applicationCacheFilePath);

        isCreated = true;
        return applicationCacheFilePath;
    }
}
