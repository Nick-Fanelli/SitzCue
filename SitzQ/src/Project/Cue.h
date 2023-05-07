#pragma once

#include "sitzqpch.h"

#include "UUID.h"

#include "Utils/YAMLConversions.h"

namespace SitzQ {

    class CueList;

    enum CueType {

        CueTypeEmpty,
        CueTypeSound

    };

    class Cue {

        friend class CueList;

    public:

        Cue() = default;
        Cue(const Cue&) = default;

        virtual void Execute();

        bool operator==(const Cue& other) const { return UUID == other.UUID; }

        CueType IdentifyCueType() const;

    public:
        struct UUID UUID;
        std::string CueName = "";
        std::optional<float> CueNumber = std::nullopt;
    };


    class SoundCue : public Cue {

        friend class CueList;

    public:
        SoundCue() = default;
        SoundCue(const SoundCue&) = default;

        void Execute() override;

    public:
        std::filesystem::path SoundFilePath;

    };

    struct CueWrapper {
        CueType Type;
        std::shared_ptr<Cue> CuePtr;
    };

}