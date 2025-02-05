#pragma once

#include "sitzqpch.h"

#include <bass.h>

#include "Asset.h"

namespace SitzQ {

    class RuntimeEngine;
    class AudioEngine;

    struct AudioSourceSpecs {

        std::optional<double> Duration = {};
        std::vector<float> Waveform;


    };

    class AudioSource : public Asset {
        
        friend class AudioEngine;

    public:
        AudioSource() = default;
        AudioSource(const std::filesystem::path& absFilePath);

        ~AudioSource() override;

        bool StreamAudio();
        void StreamFree();

        double GetCurrentPlaybackPosition() const;
        void SetCurrentPlaybackPosition(double position);
        
        void Play();
        void Loop();

        void Pause();
        void Stop();

        float GetCurrentAudioLevelMono() const;
        const std::pair<float, float>& GetCurrentAudioLevelStereo() const { return m_CurrentAudioLevel; }

        bool IsPlaying() const { return m_IsPlaying; }

        const AudioSourceSpecs& GetSpecs() const { return m_Specs; }

        const std::filesystem::path& GetAbsFilePath() const { return m_AbsFilePath; }

    private:
        void CreateAsset() override { StreamAudio(); }

        void Update();

    private:
        std::filesystem::path m_AbsFilePath;
        bool m_IsAudioStreamed = false;

        HSTREAM m_Stream;
        AudioSourceSpecs m_Specs;

        bool m_IsPlaying = false;
        std::pair<float, float> m_CurrentAudioLevel;
        double m_PlaybackPosition = 0;

    };

    class AudioEngine {

        friend class RuntimeEngine;
        friend class AudioSource;

    public:
        static const std::pair<float, float>& GetMasterAudioLevel();

    private:
        static void Initialize();
        static void Destroy();

        static void OnUpdate();

    private:    
        static inline std::vector<AudioSource*> s_AudioSources;

        static inline std::pair<float, float> s_MasterAudioLevel;

    };

}