#pragma once

#include "sitzcuepch.h"

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
        Cue(struct UUID uuid) : UUID(uuid), CueName(CreateRef<std::string>("")), CueNumber() {}
        Cue(struct UUID uuid, const std::string& cueName, float cueNumber) : UUID(uuid), CueName(CreateRef<std::string>(cueName)), CueNumber(CreateRef<float>(cueNumber)) {}

        virtual void Execute();

        bool operator==(const Cue& other) const { return UUID == other.UUID; }

    public:

        struct UUID UUID;
        Ref<std::string> CueName = CreateRef<std::string>("");
        Ref<float> CueNumber;

    };

    class CueList {

    public: 
        CueList() = default;

        Cue& CreateCue(const std::string& cueName, float cueNumber);
        Cue& CreateCue();

        Cue* GetCue(UUID uuid);

        void DeleteCue(const Cue& cue);

        const std::vector<Cue*>& GetCueCache() const { return m_SortedCuesCache; }

    private:
        UUID GenerateUUID();

        void UpdateCueCache();

        std::vector<Cue> m_Registry;
        std::vector<uint32_t> m_CueListOrder;

        std::vector<Cue*> m_SortedCuesCache;

    };

}