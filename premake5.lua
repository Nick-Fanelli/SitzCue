workspace "SitzCue"
    architecture "x86_64"
    startproject "SitzCue"

    flags {
        "MultiProcessorCompile"
    }

    configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "SitzCue/premake5.lua"