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

    struct CueNumber {

        CueNumber() = default;
        CueNumber(float value) {
            m_RawValue = value;
            m_IsAssigned = true;
        }

        operator float() const { return m_RawValue; }
        operator float&() { return m_RawValue; }

        CueNumber& operator=(float value) {
            this->m_RawValue = value;
            m_IsAssigned = true;
            return *this;
        }

        bool IsAssigned() { return m_IsAssigned; }

    private:
        float m_RawValue = -1.0f;
        bool m_IsAssigned = false;

    };

    class Cue {

        friend class CueList;

    public:

        Cue() = default;
        Cue(struct UUID uuid) : UUID(uuid), CueName(""), CueNumber() {}
        Cue(struct UUID uuid, const std::string& cueName, float cueNumber) : UUID(uuid), CueName(cueName), CueNumber(cueNumber) {}

        struct UUID UUID;
        std::string CueName = "";
        struct CueNumber CueNumber;

        bool operator==(Cue& other) const { return UUID == other.UUID; }
    };

    class CueList {

    public: 
        CueList() = default;

        Cue& CreateCue(const std::string& cueName, float cueNumber);
        Cue& CreateCue();

        Cue& GetCue(UUID uuid);

        const std::vector<Cue*>& GetCueCache() const { return m_SortedCuesCache; }

    private:
        UUID GenerateUUID();

        void UpdateCueCache();

        std::vector<Cue> m_Registry;
        std::vector<uint32_t> m_CueListOrder;

        std::vector<Cue*> m_SortedCuesCache;

    };

}