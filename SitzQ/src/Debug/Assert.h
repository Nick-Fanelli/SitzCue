#pragma once

#include "Log.h"

#if DEBUG

    #define SITZCUE_ASSERT(value) if(!value) { Log::Error("SitzQ Assertion!"); exit(-1); }
    #define SITZCUE_ASSERT_MESSAGE(value, message) if(!value) { Log::FormatError("SitzQ Assertion!:\n\t%s", message); exit(-1); }

#else

    #define SITZCUE_ASSERT(value)
    #define SITZCUE_ASSERT_MESSAGE(value, message)

#endif