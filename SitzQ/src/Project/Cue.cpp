#include "Cue.h"

using namespace SitzQ;

CueType Cue::IdentifyCueType() const {
    Cue* cue = const_cast<Cue*>(this);

    if(dynamic_cast<SoundCue*>(cue)) {
        return CueType::CueTypeSound;
    } else {
        return CueType::CueTypeEmpty;
    }
}

// ===========================================================================
// Cue
// ===========================================================================

bool Cue::IsValid() const { return true; }
bool Cue::IsActive() const { return false; }

// ===========================================================================
// Sound Cue
// ===========================================================================

void SoundCue::SetSoundFilePath(const std::filesystem::path& filepath) {
    m_SoundFilePath = filepath;
}


bool SoundCue::IsValid() const {
    return FileUtils::Exists(m_SoundFilePath);
}

bool SoundCue::IsActive() const {
    return true;
}

void SoundCue::Execute() {

    Log::Info("Execute Sound Cue");

}

void SoundCue::Stop() {

    Log::Info("Stopping Sound Cue");

}