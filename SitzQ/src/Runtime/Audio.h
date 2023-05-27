#pragma once

#include "sitzqpch.h"

#include <bass.h>

namespace SitzQ {

    class RuntimeEngine;


    struct AudioSourceSpecs {

        std::optional<double> Duration = {};
        std::vector<float> Waveform;


    };

    class AudioSource {

    public:
        AudioSource(const std::filesystem::path& absFilePath);
        ~AudioSource();

        bool StreamAudio();
        void StreamFree();

        double GetCurrentPlaybackPosition() const;
        void SetCurrentPlaybackPosition(double position);
        
        void Play();
        void Loop();

        void Pause();
        void Stop();

        float GetCurrentAudioLevel() const;

        const AudioSourceSpecs& GetSpecs() const { return m_Specs; }

    private:
        std::filesystem::path m_AbsFilePath;
        bool m_IsAudioStreamed = false;

        HSTREAM m_Stream;
        AudioSourceSpecs m_Specs;

    };

    class AudioEngine {

        friend class RuntimeEngine;

    private:
        static void Initialize();
        static void Destroy();

        static void OnUpdate(float deltaTime);

    };

}