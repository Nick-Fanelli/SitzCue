#pragma once

#include "Log.h"

#if DEBUG

    #define SITZCUE_ASSERT(value) if(!value) { Log::Error("SitzCue Assertion!"); exit(-1); }
    #define SITZCUE_ASSERT_MESSAGE(value, message) if(!value) { Log::FormatError("SitzCue Assertion!:\n\t%s", message); exit(-1); }

#else

    #define SITZCUE_ASSERT(value)
    #define SITZCUE_ASSERT_MESSAGE(value, message)

#endif