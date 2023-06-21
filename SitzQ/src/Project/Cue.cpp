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

void Cue::ReportSelfToRuntimeEngine() {

    

}

// ===========================================================================
// SoundCue
// ===========================================================================

bool SoundCue::IsValid() const {
    return FileUtils::Exists(SoundFilePath);
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