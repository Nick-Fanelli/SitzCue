#pragma once

#include "sitzcuepch.h"

namespace SitzCue {

    enum class CueType {

        CueTypeEmpty = 0

    };

    const std::string CueTypeStrings[] = {
        "Empty"
    };

    struct Cue {

        Cue() = default;
        Cue(CueType cueType, float cueNumber, const std::string& cueName) :
            CueType(cueType), CueNumber(cueNumber), CueName(cueName) {}

        CueType CueType;
        float CueNumber;
        std::string CueName;

    };

    class CueList {
    
    public:
        CueList() = default;

        Cue& CreateCue(CueType cueType, float cueNumber, const std::string& cueName);
        void SortCues();

        const std::vector<Cue>& GetCues() const { return m_Cues; }
        std::vector<Cue>& GetCues() { return m_Cues; }

    private:
        std::vector<Cue> m_Cues;

    };

}