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

    // ===========================================================================
    // Cue
    // ===========================================================================

    class Cue {

        friend class CueList;

    public:

        Cue() = default;
        Cue(const Cue&) = default;

        virtual void Execute() {}
        virtual void Stop() {}

        virtual bool IsValid() const;

        bool operator==(const Cue& other) const { return UUID == other.UUID; }

        CueType IdentifyCueType() const;

    public:
        struct UUID UUID;

        // Serializable
        std::string CueName = "";
        std::optional<float> CueNumber = std::nullopt;

        virtual bool IsActive() const;

    };

    // Cue Wrapper

    struct CueWrapper {
        CueType Type;
        std::shared_ptr<Cue> CuePtr;
    };

    // ===========================================================================
    // SoundCue
    // ===========================================================================

    class SoundCue : public Cue {

        friend class CueList;

    public:
        SoundCue() = default;
        SoundCue(const SoundCue&) = default;

        const std::filesystem::path& GetSoundFilePath() const { return m_SoundFilePath; }
        void SetSoundFilePath(const std::filesystem::path& filepath);

        void Execute() override;
        void Stop() override;

        bool IsValid() const override;
        bool IsActive() const override;

    private:
        std::filesystem::path m_SoundFilePath;

    };

}