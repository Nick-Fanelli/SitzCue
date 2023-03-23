#include "Cue.h"

using namespace SitzCue;

static bool CompareCue(const Cue& a, const Cue& b) { return a.CueNumber < b.CueNumber; }

Cue& CueList::CreateCue(CueType cueType, float cueNumber, const std::string& cueName) {
    auto& cue = m_Cues.emplace_back(cueType, cueNumber, cueName);
    SortCues();
    return cue;
}

void CueList::SortCues() {
    std::sort(m_Cues.begin(), m_Cues.end(), CompareCue);
}