project "SitzCue"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    pchheader "src/sitzcuepch.h"

    targetdir ( "%{wks.location}/build/" .. outputdir .. "/%{prj.name}")
    objdir ( "%{wks.location}/build-int/" .. outputdir .. "/%{prj.name}")

    files { "src/**.h", "src/**.hpp", "src/**.c", "src/**.cpp" }

    includedirs {
        "vendor",
        "vendor/glfw/include",
        "vendor/imgui",
        "vendor/imgui/backends",

        "src"
    }

    links {
        "GLFW",
        "GLAD",
        "ImGui"
    }

    filter "system:macosx"
        links {
            "Cocoa.framework",
            "OpenGL.framework",
            "IOKit.framework",
            "CoreVideo.framework"
        }

    filter "system:linux"
        links {
            "dl",
            "pthread"
        }

        defines {
            "_X11"
        }

    filter "system:windows"
        defines {
            "_WINDOWS"
        }

    filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

        defines {
            "DEBUG"
        }

	filter "configurations:Release"
		runtime "Release"
        optimize "On"

        defines {
            "NDEBUG"
        }
        
    filter "files:resources/**"
        buildaction ("Copy")

include "vendor/glfw/premake5.lua"
include "vendor/glad/premake5.lua"
include "vendor/imgui/premake5.lua"