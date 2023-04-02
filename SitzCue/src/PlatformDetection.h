#pragma once

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

namespace SitzCue::Platform {

    enum Platform {
        PlatformMacOS, PlatformWindows, PlatformLinux
    };

#ifdef SITZCUE_PLATFORM_WINDOWS
    inline Platform ActivePlatform = Platform::PlatformWindows;

#elif defined(SITZCUE_PLATFORM_MACOS)
    inline Platform ActivePlatform = Platform::PlatformMacOS;

#elif defined(SITZCUE_PLATFORM_LINUX)
    inline Platform ActivePlatform = Platform::PlatformLinux;
    
#endif

}
