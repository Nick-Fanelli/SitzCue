#pragma once

#include "sitzcuepch.h"

#include <yaml-cpp/yaml.h>

using namespace SitzCue;

namespace SitzCue {
    class Project;
    class CueList;
    class Cue;
}

namespace YAML {

    // Cue
    template<> struct convert<Cue*> {
        static Node encode(const Cue* cue);
    };
    
    template<> struct convert<Cue> {
        static bool decode(const Node& node, Cue& cue);
    };

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