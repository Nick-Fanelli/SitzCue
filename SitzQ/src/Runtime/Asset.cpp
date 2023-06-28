#include "Asset.h"

#include "Audio.h"

#include <condition_variable>

using namespace SitzQ;

static std::unique_ptr<std::thread> s_AssetManagerThread;

static std::atomic<bool> s_IsRunning(false);

std::shared_ptr<AssetRemote> Asset::GenerateAssetRemote() {

    SITZCUE_PROFILE_FUNCTION();

    std::shared_ptr<AssetRemote>& assetRemote = m_AssetRemotes.emplace_back(std::make_shared<AssetRemote>(this));

    return assetRemote;

}

std::shared_ptr<AssetRemote>& AssetRemote::GetNullAssetRemote() {
    
    SITZCUE_PROFILE_FUNCTION();

    static auto nullAssetRemote = std::make_shared<AssetRemote>(nullptr);

    return nullAssetRemote;
}

Asset::~Asset() {
    if(!m_AssetRemotesBroken)
        BreakAssetRemotes();
}

void Asset::BreakAssetRemotes() {

    SITZCUE_PROFILE_FUNCTION();

    for(auto& remote : m_AssetRemotes) {
        remote->m_AssetPtr = nullptr;
    }

    m_AssetRemotesBroken = true;

}

void AssetManager::InitializeAsync() {

    SITZCUE_PROFILE_FUNCTION();

    s_IsRunning = true;

    s_AssetManagerThread = std::make_unique<std::thread>([] {
        StartLoop();
    });;

    s_AssetManagerThread->detach();

}

static constexpr auto ThreadSleepDuration = std::chrono::milliseconds(20);

void AssetManager::StartLoop() {

    SITZCUE_PROFILE_FUNCTION();

    while(s_IsRunning) {

        Update();

        std::this_thread::sleep_for(ThreadSleepDuration);

    }

}

void AssetManager::Update() {

    SITZCUE_PROFILE_FUNCTION();

    if(s_WatchDirectory.has_value()) {

        static std::filesystem::file_time_type lastUpdateTime;
        FileUtils::GetLastWriteTime(s_WatchDirectory.value(), lastUpdateTime);

        if(s_LastFileUpdateTime != lastUpdateTime) {
            Log::Trace("Sweeping Directory");

            SweepDirectory();
            s_LastFileUpdateTime = lastUpdateTime;
        }

    }

}

void AssetManager::Terminate() {

    SITZCUE_PROFILE_FUNCTION();

    s_IsRunning = false;

    ClearRegistry();
}

void AssetManager::ClearRegistry() {

    SITZCUE_PROFILE_FUNCTION();

    for(auto entry : s_AssetRegistry) {
        entry.second->BreakAssetRemotes();
        delete entry.second;
    }

    s_AssetRegistry.clear();
}

static void SearchDirectory(const std::filesystem::path& path, std::vector<std::filesystem::path>& uncompressedFiles) {

    SITZCUE_PROFILE_FUNCTION();

    for(const auto& entry : std::filesystem::directory_iterator(path)) {
        if(std::filesystem::is_directory(entry)) {
            SearchDirectory(entry, uncompressedFiles);
        } else {
            uncompressedFiles.push_back(entry);
        }
    }
}

enum FileType {
    FileTypeUnidentified = 0,
    FileTypeAudio = 1
};

struct CaseInsensitiveHash {

    std::size_t operator()(const std::string& str) const {
        std::size_t hash = 0;

        for(char c : str) {
            hash = hash * 37 * std::tolower(c);
        }

        return hash;
    }

};

static FileType IdentifyFile(const std::filesystem::path& filepath) {

    SITZCUE_PROFILE_FUNCTION();

    static const std::unordered_set<std::string, CaseInsensitiveHash> audioExtensions = { ".wav", ".aiff", ".mp3", ".mp2", ".mp1", ".ogg" };

    std::string extension = filepath.extension().string();

    if(!extension.empty()) {

        if(audioExtensions.count(extension)) {
            return FileType::FileTypeAudio;
        }

    }

    return FileType::FileTypeUnidentified;

}

void AssetManager::SweepDirectory() {

    SITZCUE_PROFILE_FUNCTION();

    if(!s_WatchDirectory.has_value())
        return;

    std::vector<std::filesystem::path> allFiles;

    SearchDirectory(s_WatchDirectory.value(), allFiles); // Search Dir and get all files

    // Loop through and created any assets that don't exist
    for(auto& filepath : allFiles) {

        auto fileType = IdentifyFile(filepath);

        if(fileType == FileType::FileTypeUnidentified)
            continue;

        auto it = s_AssetRegistry.find(filepath);

        if(it != s_AssetRegistry.end())
            continue;

        Log::Trace("Creating Asset");

        // Create the asset by type
        switch(fileType) {

        case FileType::FileTypeAudio:
            Log::Trace("Creating Audio Source Asset");
            CreateAsset<AudioSource>(filepath);
            break;
        case FileType::FileTypeUnidentified:
        default:
            break;

        }
    }

    std::vector<std::string> filesMarkedForDeletion;

    // Get all files marked for deletion (aka no longer exist in the dir)
    for(auto& entry : s_AssetRegistry) {
        auto it = std::find(allFiles.begin(), allFiles.end(), entry.first);

        if(it == allFiles.end()) {
            filesMarkedForDeletion.push_back(entry.first);
        }
    }

    // Delete the files marked for deletion from the registry
    for(auto& file : filesMarkedForDeletion) {
        auto it = s_AssetRegistry.find(file);

        if(it != s_AssetRegistry.end())
            s_AssetRegistry.erase(it);
    }

}