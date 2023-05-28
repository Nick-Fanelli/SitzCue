#pragma once

#include "sitzqpch.h"

namespace SitzQ {

    class RuntimeEngine {

    public:

        static void InitializeAsync();
        static void Terminate();

        static bool IsRunningAsync();

    private:

        static void Initialize();
        static void Destroy();

        static void StartLoop();
        static void Update();

    };

}