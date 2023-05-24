#include "RuntimeEngine.h"

#include "Audio.h"

using namespace SitzQ;

void RuntimeEngine::Initialize() {
    Log::Info("Initializing the Runtime Engine...");

    AudioEngine::Initialize();

}

void RuntimeEngine::Destroy() {
    Log::Info("Destroying the Runtime Engine...");

    AudioEngine::Destroy();
}

void RuntimeEngine::Update(float deltaTime) {

}