#pragma once

#include "sitzqpch.h"

namespace SitzQ {

    class AudioSource;
    class AssetRemote;
    class AssetManager;

    class Asset {

        friend class AssetManager;
        friend class AssetRemote;
    
    public:
        virtual ~Asset();

    private:
        std::shared_ptr<AssetRemote> GenerateAssetRemote();

        void BreakAssetRemotes();
        virtual void CreateAsset() = 0;

    private:
        std::vector<std::shared_ptr<AssetRemote>> m_AssetRemotes;
        bool m_AssetRemotesBroken = false;

    };

    class AssetRemote {

        friend class Asset;

    public:

        AssetRemote() = default;
        AssetRemote(Asset* assetPtr) : m_AssetPtr(assetPtr) {}

        static std::shared_ptr<AssetRemote>& GetNullAssetRemote();

        Asset* GetAssetPtr() const { return m_AssetPtr; }

    private:

        Asset* m_AssetPtr = nullptr;

    };

    template<typename T>
    class AssetHandle {

    public:
        AssetHandle() = default;
        AssetHandle(std::shared_ptr<AssetRemote>& assetRemote) : m_AssetRemote(assetRemote) {}

        std::optional<T*> Get() const { 

            SITZCUE_PROFILE_FUNCTION();

            if(m_AssetRemote.get() != nullptr) {
                T* ptr = dynamic_cast<T*>(m_AssetRemote->GetAssetPtr());

                if(ptr != nullptr) {
                    return std::optional<T*>(ptr);
                }
            }
            return {};
        }

        bool IsNull() const { 

            SITZCUE_PROFILE_FUNCTION();

            if(m_AssetRemote.get() != nullptr)
                return m_AssetRemote->GetAssetPtr() == nullptr; 
            return true;
        }

        operator bool() const { 

            SITZCUE_PROFILE_FUNCTION();

            if(m_AssetRemote.get() != nullptr)
                return m_AssetRemote->GetAssetPtr() != nullptr; 
            return false; 
        }

    private:

        std::shared_ptr<AssetRemote> m_AssetRemote;
        
    };

    class AssetManager {

    public: 
        static void InitializeAsync();
        static void Terminate();

        static void ClearRegistry();

        static void AssignWatchDirectory(const std::filesystem::path& path) {
            s_WatchDirectory = path;
            s_LastFileUpdateTime = std::filesystem::file_time_type();
        }

        template<typename T>
        static AssetHandle<T> CreateAsset(const std::filesystem::path& filepath) {

            SITZCUE_PROFILE_FUNCTION();

            if( !std::is_base_of<Asset, typename std::decay<T>::type>::value &&
                !std::is_same<Asset, typename std::decay<T>::type>::value) {
                Log::FormatError("When creating asset, \"%s\", the base type does not inherit Asset\n\tStatus: Returning Null Asset Handle", filepath.c_str());
                return AssetHandle<T>(AssetRemote::GetNullAssetRemote());
            }

            Asset* assetPtr = new T(filepath);
            assetPtr->CreateAsset(); // TODO: Multithread by assigning to other thread and then wait to create by setting remote to nullptr until created
            s_AssetRegistry[filepath] = assetPtr;

            std::shared_ptr<AssetRemote> remote = assetPtr->GenerateAssetRemote();
            AssetHandle handle = AssetHandle<T>(remote);

            return handle;
        }

    private:
        static void SweepDirectory();

        static void StartLoop();
        static void Update();


    private:
        static inline std::optional<std::filesystem::path> s_WatchDirectory = {};
        static inline std::filesystem::file_time_type s_LastFileUpdateTime;

        static inline std::unordered_map<std::string, Asset*> s_AssetRegistry;

    };
    
}