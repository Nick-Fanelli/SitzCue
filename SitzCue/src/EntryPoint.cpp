#include "sitzcuepch.h"

#include <nfd.hpp>

#include "Application/Application.h"

using namespace SitzCue;

int main() {

    SITZCUE_PROFILE_BEGIN_SESSION("SitzCue", "SitzCueProfilingResults.json");

    // Initialize Global Libraries
    NFD_Init();

    Application application = Application();
    application.StartApplication();

    NFD_Quit();
    
    SITZCUE_PROFILE_END_SESSION();

}