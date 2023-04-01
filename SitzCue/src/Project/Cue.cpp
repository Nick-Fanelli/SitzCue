#include "Cue.h"

using namespace SitzCue;

static std::random_device randomDevice;

Cue& CueList::CreateCue(const std::string& name, float cueNumber) {

    UUID uuid = GenerateUUID();

    Cue& cue = m_Registry.emplace_back(uuid, name, cueNumber);
    m_CueListOrder.push_back(uuid);

    UpdateCueCache();

    return cue;

}

Cue& CueList::CreateCue() {
    UUID uuid = GenerateUUID();

    Cue& cue = m_Registry.emplace_back(uuid);
    m_CueListOrder.push_back(uuid);

    UpdateCueCache();

    return cue;
}

UUID CueList::GenerateUUID() {

    bool isUnique = false;
    uint32_t rand = 0;

    while(!isUnique) {

        std::mt19937 gen(randomDevice());
        std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX);

        rand = dis(gen);
        bool isDuplicate = false;

        // Ensure no duplicate UUIDs
        for(Cue& cue : m_Registry) {
            if(cue.UUID == rand) {
                Log::Info("Duplicate UUID Found!");
                isDuplicate = true;
                break;
            }
        }

        isUnique = !isDuplicate;
    }

    return UUID(rand);
}

void CueList::UpdateCueCache() {

    m_SortedCuesCache.clear();

    for(uint32_t uuid : m_CueListOrder) {
        for(Cue& cue : m_Registry) {
            if(cue.UUID == uuid) {
                m_SortedCuesCache.push_back(&cue);
                break;
            }
        }
    }

}

