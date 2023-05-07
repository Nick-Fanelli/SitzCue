#pragma once

#include "sitzqpch.h"

#include <yaml-cpp/yaml.h>

using namespace SitzQ;

namespace SitzQ {
    class Project;
    class CueList;

    class Cue;
    class SoundCue;

    class CueWrapper;
}

namespace YAML {

    // Filesystem Path
    template<> struct convert<std::filesystem::path> {
        static Node encode(const std::filesystem::path& cueList);
        static bool decode(const Node& node, std::filesystem::path& cueList);
    };

    // CueWrapper
    template<> struct convert<CueWrapper> {
        static Node encode(const CueWrapper& cueList);
        static bool decode(const Node& node, CueWrapper& cueList);
    };

    // Cue*
    template<> struct convert<Cue*> {
        static Node encode(const Cue* cue);
    };
    
    // Cue
    template<> struct convert<Cue> { static bool decode(const Node& node, Cue& cue); };
    template<> struct convert<SoundCue> { static bool decode(const Node& node, SoundCue& cue); };

    // CueList
    template<> struct convert<CueList> {
        static Node encode(const CueList& cueList);
        static bool decode(const Node& node, CueList& cueList);
    };

    // Project
    template<>
    struct convert<Project> {
        static Node encode(const Project& project);
        static bool decode(const Node& node, Project& project);
    };


}