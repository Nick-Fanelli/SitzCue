#include "sitzqpch.h"

#include <nfd.hpp>

#include "Application/Application.h"
#include "Utils/YAMLConversions.h"

using namespace SitzQ;

int main() {

    SITZCUE_PROFILE_BEGIN_SESSION("SitzQ", "SitzQProfilingResults.json");

    // Initialize Global Libraries
    NFD_Init();

    Application application = Application();
    application.StartApplication();

    NFD_Quit();
    
    SITZCUE_PROFILE_END_SESSION();

}