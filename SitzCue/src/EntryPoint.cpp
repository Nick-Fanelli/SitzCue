#include "sitzcuepch.h"

#include "Application/Application.h"

using namespace SitzCue;

int main() {

    SITZCUE_PROFILE_BEGIN_SESSION("SitzCue", "SitzCueProfilingResults.json");

    Application application = Application();
    application.StartApplication();
    
    SITZCUE_PROFILE_END_SESSION();

}