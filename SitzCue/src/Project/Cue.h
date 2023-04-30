#pragma once

#include "sitzcuepch.h"

#include "UUID.h"

#include "Utils/YAMLConversions.h"

namespace SitzCue {

    class CueList;

    class Cue {

        friend class CueList;

    public:

        Cue() = default;
        Cue(struct UUID uuid) : UUID(uuid), CueName(""), CueNumber(std::nullopt) {}
        Cue(struct UUID uuid, const std::string& cueName, float cueNumber) : UUID(uuid), CueName(cueName), CueNumber(cueNumber) {}

        virtual void Execute() {}

        bool operator==(const Cue& other) const { return UUID == other.UUID; }

    public:

        struct UUID UUID;
        std::string CueName = "";
        std::optional<float> CueNumber = std::nullopt;

    };

}