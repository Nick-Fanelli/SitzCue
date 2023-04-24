#pragma once

#include "sitzcuepch.h"

#include "Utils/YAMLConversions.h"

namespace SitzCue {

    struct UUID {

    public:
        UUID() = default;
        UUID(uint32_t uuid) : m_UUID(uuid) {}

        operator uint32_t() const { return m_UUID; }

        bool operator==(UUID& other) const { return other.m_UUID == m_UUID; }

    private:

        uint32_t m_UUID;

    };

    class CueList;

    class Cue {

        friend class CueList;

    public:

        Cue() = default;
        Cue(struct UUID uuid) : UUID(uuid), CueName(""), CueNumber(std::nullopt) {}
        Cue(struct UUID uuid, const std::string& cueName, float cueNumber) : UUID(uuid), CueName(cueName), CueNumber(cueNumber) {}

        virtual void Execute();

        bool operator==(const Cue& other) const { return UUID == other.UUID; }

    public:

        struct UUID UUID;
        std::string CueName = "";
        std::optional<float> CueNumber = std::nullopt;

    };

    class CueList {

        friend struct YAML::convert<CueList>;
        
    public: 
        CueList() = default;

        std::shared_ptr<Cue> CreateCue(const std::string& cueName, float cueNumber);
        std::shared_ptr<Cue> CreateCue();

        std::shared_ptr<Cue> CreateCue(UUID followCueUUID);
        std::shared_ptr<Cue> CreateCue(size_t location);

        void MoveCue(UUID targetCue, UUID cueToFollow);

        void ReinstateCue(const std::shared_ptr<Cue>& cue, uint32_t position);
        void ReinstateCue(const std::shared_ptr<Cue>& cue);

        uint32_t GetListPositionOfCue(const std::shared_ptr<Cue>& cue);

        std::shared_ptr<Cue> GetCue(UUID uuid);

        void DeleteCue(const std::shared_ptr<Cue>& cue);

        const std::vector<Cue*>& GetCueCache() const { return m_SortedCuesCache; }
        
    private:
        UUID GenerateUUID();

        void UpdateCueCache();

        std::vector<std::shared_ptr<Cue>> m_Registry;
        std::vector<uint32_t> m_CueListOrder;

        std::vector<Cue*> m_SortedCuesCache;

    };

}