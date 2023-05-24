#pragma once

#include "sitzqpch.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "alhelpers.h"

namespace SitzQ::RuntimeEngine {

    namespace AudioEngine {

        void Initialize();

        static ALCdevice* s_AudioDevice = nullptr;

    }

}