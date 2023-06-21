#include "RuntimeEngine.h"

#include <atomic>
#include <condition_variable>

#include "Audio.h"

#include "Project/Cue.h"

using namespace SitzQ;

// ===========================================================================
// RuntimeEngine
// ===========================================================================

static std::shared_ptr<std::thread> s_RuntimeEngineThread;

static std::atomic<bool> s_IsRunning(false);
static std::atomic<bool> s_IsTerminated(false);

static std::condition_variable s_ConditionalVariable;
static std::mutex s_Mutex;

static bool s_Interrupted = false;

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
    s_Interrupted = true;

    const auto sleepTime = std::chrono::milliseconds(1);

    while(!s_IsTerminated) {
        std::this_thread::sleep_for(sleepTime);
    }
}

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

            {
                std::unique_lock<std::mutex> lock(s_Mutex);
                s_ConditionalVariable.wait_for(lock, sleepDuration, [] { return s_Interrupted; });
            }

            if(s_Interrupted) {
                s_Interrupted = false;
            }

            // std::this_thread::sleep_for(sleepDuration);
        }

    }

    Destroy();

    s_IsTerminated = true;

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