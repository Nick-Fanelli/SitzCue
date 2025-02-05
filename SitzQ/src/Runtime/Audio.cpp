#include "Audio.h"

#include "RuntimeEngine.h"

using namespace SitzQ;

AudioSource::AudioSource(const std::filesystem::path& absFilePath) : m_AbsFilePath(absFilePath) {
    AudioEngine::s_AudioSources.push_back(this);
}

AudioSource::~AudioSource() {

    SITZCUE_PROFILE_FUNCTION();

    AudioEngine::s_AudioSources.erase(std::remove(
        AudioEngine::s_AudioSources.begin(),
        AudioEngine::s_AudioSources.end(),
        this
    ));

    if(m_IsAudioStreamed)
        StreamFree();
}

bool AudioSource::StreamAudio() {

    SITZCUE_PROFILE_FUNCTION();

    if(m_IsAudioStreamed) {
        Log::Warn("Attempting to re-stream audio\n\tStatus: Returning");
        return false;
    }

    if(!FileUtils::Exists(m_AbsFilePath.c_str())) {
        Log::Error("Audio file does not exist");
        return false;
    }

    m_Stream = BASS_StreamCreateFile(FALSE, m_AbsFilePath.c_str(), 0, 0, BASS_SAMPLE_FLOAT);

    if(m_Stream == 0) {
        Log::Error("Could not stream file!");
        return false;
    }

    m_Specs = {};

    // Load Specs
    QWORD length = BASS_ChannelGetLength(m_Stream, BASS_POS_BYTE);
    double duration = BASS_ChannelBytes2Seconds(m_Stream, length);
    m_Specs.Duration = duration;

    m_IsAudioStreamed = true;

    return true;
}

void AudioSource::StreamFree() {

    SITZCUE_PROFILE_FUNCTION();

    if(!m_IsAudioStreamed) {
        Log::Warn("Not streamed audio is attempting to be freed\n\tStatus: Returning");
        return;
    }

    BASS_StreamFree(m_Stream);
    m_Specs = {};
    m_IsAudioStreamed = false;

}

static float fft[1];

float AudioSource::GetCurrentAudioLevelMono() const {

    return (m_CurrentAudioLevel.first + m_CurrentAudioLevel.second) / 2.0f;

}

double AudioSource::GetCurrentPlaybackPosition() const {
    return m_PlaybackPosition;
}

void AudioSource::SetCurrentPlaybackPosition(double position) {
    auto pos = BASS_ChannelSeconds2Bytes(m_Stream, position);
    BASS_ChannelSetPosition(m_Stream, pos, BASS_POS_BYTE);
}

void AudioSource::Play() {
    BASS_ChannelPlay(m_Stream, FALSE);
}

void AudioSource::Loop() {
    BASS_ChannelPlay(m_Stream, TRUE);
}

void AudioSource::Pause() {
    BASS_ChannelStop(m_Stream);
}

void AudioSource::Stop() {
    Pause();
    BASS_ChannelSetPosition(m_Stream, 0, BASS_POS_BYTE);
}

void AudioSource::Update() {

    SITZCUE_PROFILE_FUNCTION();

    m_IsPlaying = BASS_ChannelIsActive(m_Stream);

    DWORD level = BASS_ChannelGetLevel(m_Stream);
    m_CurrentAudioLevel = { LOWORD(level) / 32768.0f, HIWORD(level) / 32768.0f };

    auto position = BASS_ChannelGetPosition(m_Stream, BASS_POS_BYTE);
    m_PlaybackPosition = BASS_ChannelBytes2Seconds(m_Stream, position);
}

void AudioEngine::Initialize() {

    SITZCUE_PROFILE_FUNCTION();

    Log::Trace("Initializing the Audio Engine...");

    Log::Trace("BASS Init");
    if(!BASS_Init(-1, 44100, 0, nullptr, nullptr)) {
        Log::Error("Problem Initializing BASS");
        return;
    }

}

void AudioEngine::Destroy() {
    Log::Trace("Destroying the Audio Engine...");

    Log::Trace("Bass Free");
    BASS_Free();
}

void AudioEngine::OnUpdate() {
    SITZCUE_PROFILE_FUNCTION();

    BASS_Update(RuntimeEngine::UpdateSecondInterval);

    std::pair<float, float> levelSum = { 0.0f, 0.0f };
    uint32_t activeChannels = 0;

    // Calculate master audio level
    for(auto* source : s_AudioSources) {

        source->Update();

        if(!source->IsPlaying())
            continue;

        auto& level = source->GetCurrentAudioLevelStereo();

        levelSum.first += level.first;
        levelSum.second += level.second;

        activeChannels++;
    }


    if(activeChannels > 0) {
        levelSum.first /= activeChannels;
        levelSum.second /= activeChannels;
    }

    s_MasterAudioLevel = levelSum;
}

const std::pair<float, float>& AudioEngine::GetMasterAudioLevel() { return s_MasterAudioLevel; }