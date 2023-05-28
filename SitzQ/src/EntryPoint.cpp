#include "EntryPoint.h"

#include <nfd.hpp>

#include "Application/Application.h"
#include "Runtime/RuntimeEngine.h"
#include "Utils/YAMLConversions.h"

using namespace SitzQ;

EntryPoint::EntryPoint() {
    NFD_Init();

    RuntimeEngine::InitializeAsync();    
}

EntryPoint::~EntryPoint() {
    NFD_Quit();

    RuntimeEngine::Terminate();
}

int main() {

    SITZCUE_PROFILE_BEGIN_SESSION("SitzQ", "SitzQProfilingResults.json");

    EntryPoint* entryPoint = new EntryPoint();

    Application application = Application();
    application.StartApplication();

    delete entryPoint;

    SITZCUE_PROFILE_END_SESSION();

}