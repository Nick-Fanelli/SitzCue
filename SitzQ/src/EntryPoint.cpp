
#include "Application/Application.h"

#include "Runtime/RuntimeEngine.h"
#include "Utils/YAMLConversions.h"

int main() {

    SITZCUE_PROFILE_BEGIN_SESSION("SitzQ", "SitzQProfilingResults.json");

    Application application = Application();
    application.StartApplication();

    SITZCUE_PROFILE_END_SESSION();

}