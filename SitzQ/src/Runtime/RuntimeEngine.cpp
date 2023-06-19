#include "RuntimeEngine.h"

#include <atomic>

#include "Audio.h"

using namespace SitzQ;

static std::shared_ptr<std::thread> s_RuntimeEngineThread;
static std::atomic<bool> s_IsRunning(false);

bool RuntimeEngine::IsRunningAsync() { return s_IsRunning; }

void RuntimeEngine::InitializeAsync() {

    SITZCUE_PROFILE_FUNCTION();

    Initialize();

    s_IsRunning = true;
    s_RuntimeEngineThread = std::make_shared<std::thread>([] {
        StartLoop();
    });
    s_RuntimeEngineThread->detach();

}

void RuntimeEngine::Terminate() {

    s_IsRunning = false;

}

static constexpr uint32_t UpdatesPerSecond = 30;
static constexpr double UpdateSecondInterval = 1.0 / UpdatesPerSecond;

void RuntimeEngine::StartLoop() {

    SITZCUE_PROFILE_FUNCTION();

    while(s_IsRunning) {

        double startTime = glfwGetTime(); // Seconds

        Update();

        double endTime = glfwGetTime(); // Seconds
        
        double elapsed = endTime - startTime;
        double remainingTime = UpdateSecondInterval - elapsed;

        if(remainingTime > 0) {
            std::chrono::duration<double> sleepDuration(remainingTime);
            std::this_thread::sleep_for(sleepDuration);
        }

    }

    Destroy();

}

void RuntimeEngine::Initialize() {
    AudioEngine::Initialize();
}

void RuntimeEngine::Update() {
    SITZCUE_PROFILE_FUNCTION();

    AudioEngine::OnUpdate();
}

void RuntimeEngine::Destroy() {
    AudioEngine::Destroy();
}