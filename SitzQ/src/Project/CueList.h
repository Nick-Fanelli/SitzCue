#pragma once

#include "sitzqpch.h"

#include "Cue.h"

namespace SitzQ {

    class CueList {

        friend struct YAML::convert<CueList>;
        
    public: 
        CueList() = default;

        std::shared_ptr<Cue> PushBackCueToRegistry(CueType cueType);

        std::shared_ptr<Cue> CreateCue(CueType cueType);

        std::shared_ptr<Cue> CreateCueAt(CueType cueType, UUID cueUUIDToFollow);
        std::shared_ptr<Cue> CreateCueAt(CueType cueType, size_t location);

        void MoveCue(UUID targetCue, UUID cueToFollow);

        void ReinstateCue(const std::shared_ptr<Cue>& cue, uint32_t position);
        void ReinstateCue(const std::shared_ptr<Cue>& cue);

        uint32_t GetListPositionOfCue(const std::shared_ptr<Cue>& cue);

        uint32_t FirstUUID() const;
        uint32_t LastUUID() const;

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