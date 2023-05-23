#pragma once

#include "sitzqpch.h"

namespace SitzQ {

    namespace RuntimeEngine {

        void Initialize();
        void Destroy();

        void Update(float deltaTime);

    }

}