#include "Asset.h"

using namespace SitzQ;

static std::unique_ptr<std::thread> s_AssetManagerThread;
static std::atomic<bool> s_IsRunning(false);

AudioSource* AssetManager::CreateAudioSource(const std::filesystem::path& path) {

    SITZCUE_PROFILE_FUNCTION();

    AudioSource* audioSource = new AudioSource(path);
    audioSource->StreamAudio();

    s_AudioSources[std::string(path)] = audioSource;

    return audioSource;
}

AudioSource* AssetManager::GetAudioSource(const std::filesystem::path& path) {
    SITZCUE_PROFILE_FUNCTION();

    auto it = s_AudioSources.find(path);

    if(it != s_AudioSources.end()) {
        return it->second;
    }

    return nullptr;
}

void AssetManager::InitializeAsync() {

    SITZCUE_PROFILE_FUNCTION();

    s_IsRunning = true;

    s_AssetManagerThread = std::make_unique<std::thread>([] {
        StartLoop();
    });;

    s_AssetManagerThread->detach();

}

static constexpr auto ThreadSleepDuration = std::chrono::seconds(1);

void AssetManager::StartLoop() {

    SITZCUE_PROFILE_FUNCTION();

    while(s_IsRunning) {

        Update();

        std::this_thread::sleep_for(ThreadSleepDuration);

    }

}

void AssetManager::Update() {

    SITZCUE_PROFILE_FUNCTION();

    SweepDirectory();

}

void AssetManager::Terminate() {

    SITZCUE_PROFILE_FUNCTION();

    s_IsRunning = false;

    for(auto audioSource : s_AudioSources) {
        delete audioSource.second;
    }

    s_AudioSources.clear();

}

static void SearchDirectory(const std::filesystem::path& path, std::vector<std::filesystem::path>& uncompressedFiles) {
    for(const auto& entry : std::filesystem::directory_iterator(path)) {
        if(std::filesystem::is_directory(entry)) {
            SearchDirectory(entry, uncompressedFiles);
        } else {
            uncompressedFiles.push_back(entry);
        }
    }
}

void AssetManager::SweepDirectory() {

    SITZCUE_PROFILE_FUNCTION();

    if(!s_WatchDirectory.has_value())
        return;

    std::vector<std::filesystem::path> uncompressedFiles;

    SearchDirectory(s_WatchDirectory.value(), uncompressedFiles);

    for(auto& file : uncompressedFiles) {
        Log::Info(file);
    }

}