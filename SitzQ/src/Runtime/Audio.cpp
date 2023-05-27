#include "Audio.h"

using namespace SitzQ;

AudioSource::AudioSource(const std::filesystem::path& absFilePath) : m_AbsFilePath(absFilePath) {

}

AudioSource::~AudioSource() {
    if(m_IsAudioStreamed)
        StreamFree();
}

bool AudioSource::StreamAudio() {

    SITZCUE_PROFILE_FUNCTION();

    if(m_IsAudioStreamed) {
        Log::Warn("Attempting to re-stream audio\n\tStatus: Returning");
        return false;
    }

    m_Stream = BASS_StreamCreateFile(FALSE, m_AbsFilePath.c_str(), 0, 0, 0);

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

float AudioSource::GetCurrentAudioLevel() const {

    DWORD level = BASS_ChannelGetLevel(m_Stream);

    // Extract the left and right channel volumes
    float leftLevel = LOWORD(level) / 32768.0f;   // Left channel volume (0.0 to 1.0)
    float rightLevel = HIWORD(level) / 32768.0f;  // Right channel volume (0.0 to 1.0)

    return (leftLevel + rightLevel) / 2.0f;

}

double AudioSource::GetCurrentPlaybackPosition() const {
    auto position = BASS_ChannelGetPosition(m_Stream, BASS_POS_BYTE);
    return BASS_ChannelBytes2Seconds(m_Stream, position);
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

void AudioEngine::Initialize() {

    Log::Info("Initializing the Audio Engine...");

    if(!BASS_Init(-1, 44100, 0, nullptr, nullptr)) {
        Log::Error("Problem Initializing BASS");
        return;
    }
}

void AudioEngine::Destroy() {
    Log::Info("Destroying the Audio Engine...");

    BASS_Free();
}

void AudioEngine::OnUpdate(float deltaTime) {
    BASS_Update(deltaTime);
}