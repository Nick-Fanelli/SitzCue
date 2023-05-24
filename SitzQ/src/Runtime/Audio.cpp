#include "Audio.h"

using namespace SitzQ::RuntimeEngine;

void AudioEngine::Initialize() {

    if(InitAL(s_AudioDevice, nullptr, 0) != 0) {
        Log::Error("Audio Device Error!");
        return;
    }

    

}