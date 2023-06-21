#pragma once

#include "sitzqpch.h"

namespace SitzQ {

    class Cue;

    // ===========================================================================
    // RuntimeEngine
    // ===========================================================================

    class RuntimeEngine {

    public:

        static void InitializeAsync();
        static void Terminate();

        static bool IsRunningAsync();

    public:
        static constexpr uint32_t UpdatesPerSecond = 30;
        static constexpr double UpdateSecondInterval = 1.0 / UpdatesPerSecond;

    private:

        static void Initialize();
        static void Destroy();

        static void StartLoop();

        static void Update();

    private:

    };

}