#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

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

namespace SitzCue::PlatformDetection {

    enum Platform {
        PlatformMacOS, PlatformWindows, PlatformLinux
    };

#ifdef SITZCUE_PLATFORM_WINDOWS
    inline Platform ActivePlatform = Platform::PlatformWindows;

    inline bool IsNativeCommandKey() {
        static const auto& io = ImGui::GetIO();
        return io.KeyCtrl;
    }

#elif defined(SITZCUE_PLATFORM_MACOS)
    inline Platform ActivePlatform = Platform::PlatformMacOS;

    inline bool IsNativeCommandKey() {
        static const auto& io = ImGui::GetIO();
        return io.KeySuper;
    }

#elif defined(SITZCUE_PLATFORM_LINUX)
    inline Platform ActivePlatform = Platform::PlatformLinux;

    inline bool IsNativeCommandKey() {
        static const auto& io = ImGui::GetIO();
        return io.KeyCtrl;
    }
#endif

}
