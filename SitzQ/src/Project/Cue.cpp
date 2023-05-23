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

// Cue Validation
bool Cue::IsValid() const { return true; }

bool SoundCue::IsValid() const {
    return FileUtils::Exists(SoundFilePath);
}

// Cue
void Cue::Execute() {
    Log::Info("Execute Cue");
}

void SoundCue::Execute() {

    Log::Info("Execute Sound Cue");

}