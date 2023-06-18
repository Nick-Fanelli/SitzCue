#pragma once

#include "sitzqpch.h"

#include "Audio.h"

namespace SitzQ {

    class AssetManager {

    public: 
        static AudioSource* CreateAudioSource(const std::filesystem::path& path);

        static AudioSource* GetAudioSource(const std::filesystem::path& path);

        static void InitializeAsync();
        static void Terminate();

        static void AssignWatchDirectory(const std::filesystem::path& path) {
            s_WatchDirectory = path;
        }

    private:
        static void SweepDirectory();

        static void StartLoop();
        static void Update();

    private:
        static inline std::unordered_map<std::string, AudioSource*> s_AudioSources;

        static inline std::optional<std::filesystem::path> s_WatchDirectory = {};

    };
    
}