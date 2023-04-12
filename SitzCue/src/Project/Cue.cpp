#include "Cue.h"

using namespace SitzCue;

static std::random_device randomDevice;

void Cue::Execute() {
    
}

std::shared_ptr<Cue> CueList::CreateCue(const std::string& name, float cueNumber) {

    SITZCUE_PROFILE_FUNCTION();

    UUID uuid = GenerateUUID();

    auto cue = m_Registry.emplace_back(std::make_shared<Cue>(uuid, name, cueNumber));
    m_CueListOrder.push_back(uuid);

    UpdateCueCache();

    return cue;

}

std::shared_ptr<Cue> CueList::CreateCue() {

    SITZCUE_PROFILE_FUNCTION();

    UUID uuid = GenerateUUID();

    auto cue = m_Registry.emplace_back(std::make_shared<Cue>(uuid));
    m_CueListOrder.push_back(uuid);

    UpdateCueCache();

    return cue;
}

void CueList::ReinstateCue(const std::shared_ptr<Cue>& cue, uint32_t position) {

    SITZCUE_PROFILE_FUNCTION();

    m_Registry.push_back(cue);
    m_CueListOrder.insert(m_CueListOrder.begin() + position, cue->UUID);

    UpdateCueCache();

}

void CueList::ReinstateCue(const std::shared_ptr<Cue>& cue) { ReinstateCue(cue, m_CueListOrder.size() - 1); }

uint32_t CueList::GetListPositionOfCue(const std::shared_ptr<Cue>& cue) {

    auto it = std::find(m_CueListOrder.begin(), m_CueListOrder.end(), cue->UUID);
    if(it != m_CueListOrder.end()) {
        uint32_t position = static_cast<uint32_t>(std::distance(m_CueListOrder.begin(), it));
        return position;
    }

    Log::Error("Could not find the position of a cue");
    return 0;
}

std::shared_ptr<Cue> CueList::GetCue(UUID uuid) {

    SITZCUE_PROFILE_FUNCTION();

    for(auto& cue : m_Registry) {
        if(cue->UUID == uuid) {
            return cue;
        }
    }

    return nullptr;

}

void CueList::DeleteCue(const std::shared_ptr<Cue>& cue) {

    SITZCUE_PROFILE_FUNCTION();

    // Remove from Cue List Order
    auto uuidIterator = std::find(m_CueListOrder.begin(), m_CueListOrder.end(), cue->UUID);
    if(uuidIterator != m_CueListOrder.end()) {
        m_CueListOrder.erase(uuidIterator);
    }

    // Delete from Registry
    auto it = std::remove(m_Registry.begin(), m_Registry.end(), cue);
    if(it != m_Registry.end())
        m_Registry.erase(it);


    // Update the Cue-Cache
    UpdateCueCache();
}

UUID CueList::GenerateUUID() {

    SITZCUE_PROFILE_FUNCTION();

    bool isUnique = false;
    uint32_t rand = 0;

    while(!isUnique) {

        std::mt19937 gen(randomDevice());
        std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX);

        rand = dis(gen);
        bool isDuplicate = false;

        // Ensure no duplicate UUIDs
        for(auto& cue : m_Registry) {
            if(cue->UUID == rand) {
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

    SITZCUE_PROFILE_FUNCTION();

    m_SortedCuesCache.clear();

    for(uint32_t uuid : m_CueListOrder) {
        bool isFound = false;

        for(auto& cue : m_Registry) {
            if(cue->UUID == uuid) {
                m_SortedCuesCache.push_back(&*cue);
                isFound = true;
                break;
            }
        }

        if(!isFound) {
            Log::Warn("Found cue in cue list order that does not exist in registry. \n\tStatus: Deleting from Cue List Order Vector");

            auto it = std::find(m_CueListOrder.begin(), m_CueListOrder.end(), uuid);
            if(it != m_CueListOrder.end())
                m_CueListOrder.erase(it);

            continue;
        }
    }

    // Safety Check for Un-targeted cues in Registry
    for(auto& cue : m_Registry) {

        bool isFound = false;

        for(uint32_t uuid : m_CueListOrder) {
            if(cue->UUID == uuid) {
                isFound = true;
                break;
            }
        }

        if(!isFound) {
            Log::Warn("Found cue in the registry that is not referenced by the cue list order.\n\tStatus: Deleting Cue from Registry");

            auto it = std::find(m_Registry.begin(), m_Registry.end(), cue);
            if(it != m_Registry.end())
                m_Registry.erase(it);
        }

    }

}

