#pragma once

#include "sitzcuepch.h"

#include "Cue.h"

namespace SitzCue {

    class Project {

    public:
        static void CreateNewProject();

    public:
        Project() = default;

        const CueList& GetCueList() const { return m_CueList; }
        CueList& GetCueList() { return m_CueList; }

    private:
        CueList m_CueList;

    };

}