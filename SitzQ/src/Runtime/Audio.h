#pragma once

#include "sitzqpch.h"

#include <bass.h>

namespace SitzQ {

    class RuntimeEngine;

    class AudioSource {

    public:
        AudioSource(const std::filesystem::path& absFilePath);
        ~AudioSource();

        bool StreamAudio();
        void StreamFree();

        void Play();
        void Loop();

        void Pause();
        void Stop();

    private:
        std::filesystem::path m_AbsFilePath;
        bool m_IsAudioStreamed = false;

        HSTREAM m_Stream;

    };

    class AudioEngine {

        friend class RuntimeEngine;

    private:
        static void Initialize();
        static void Destroy();

    };

}