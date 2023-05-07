workspace "SitzQ"
    architecture "x86_64"
    startproject "SitzQ"

    flags {
        "MultiProcessorCompile"
    }

    configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "SitzQ/premake5.lua"