#pragma once

#include "sitzqpch.h"

namespace SitzQ {

    class Application;
    class EntryPoint;

    class RuntimeEngine {
        
        friend class Application;
        friend class EntryPoint;

    public:

    private:

        static void Initialize();
        static void Destroy();

        static void Update(float deltaTime);

    };

}