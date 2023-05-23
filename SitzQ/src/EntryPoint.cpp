#include "sitzqpch.h"

#include <nfd.hpp>

#include "Application/Application.h"
#include "Runtime/RuntimeEngine.h"
#include "Utils/YAMLConversions.h"

using namespace SitzQ;

static inline void Initialize() {
    NFD_Init();
    
    RuntimeEngine::Initialize();
}

static inline void Destroy() {
    NFD_Quit();

    RuntimeEngine::Destroy();    
}

int main() {

    SITZCUE_PROFILE_BEGIN_SESSION("SitzQ", "SitzQProfilingResults.json");
    Initialize();

    Application application = Application();
    application.StartApplication();

    Destroy();
    SITZCUE_PROFILE_END_SESSION();

}