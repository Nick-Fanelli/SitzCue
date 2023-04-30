#pragma once

#include "sitzcuepch.h"

#include "Cue.h"

namespace SitzCue {

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