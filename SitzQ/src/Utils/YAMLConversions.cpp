#include "YAMLConversions.h"

#include "Project/Cue.h"
#include "Project/Project.h"

using namespace SitzCue;
using namespace YAML;

// ====================================================================
// Filesystem Path
// ====================================================================

Node convert<std::filesystem::path>::encode(const std::filesystem::path& filepath) {

    Node node;

    node = filepath.string();

    return node;

}

bool convert<std::filesystem::path>::decode(const Node& node, std::filesystem::path& filepath) {

    filepath = std::filesystem::path(node.as<std::string>());

    return true;

}

// ====================================================================
// Cue*
// ====================================================================
Node convert<Cue*>::encode(const Cue* constCue) {
    Node node;

    Cue* cue = const_cast<Cue*>(constCue);

    node["UUID"] = static_cast<uint32_t>(cue->UUID);
    node["Cue Name"] = cue->CueName;

    if(cue->CueNumber.has_value())
        node["Cue Number"] = cue->CueNumber.value();

    if(dynamic_cast<SoundCue*>(cue)) {
        SoundCue* soundCue = dynamic_cast<SoundCue*>(cue);

        node["Sound File Path"] = soundCue->SoundFilePath;
    }

    return node;
}


// ====================================================================
// Cue
// ====================================================================
bool convert<Cue>::decode(const Node& node, Cue& cue) {

    if(node["UUID"])
        cue.UUID = UUID(node["UUID"].as<uint32_t>());

    if(node["Cue Name"])
        cue.CueName = node["Cue Name"].as<std::string>();

    if(node["Cue Number"])
        cue.CueNumber = { node["Cue Number"].as<float>() };

    return true;
}

bool convert<SoundCue>::decode(const Node& node, SoundCue& cue) {

    if(node["UUID"])
        cue.UUID = UUID(node["UUID"].as<uint32_t>());

    if(node["Cue Name"])
        cue.CueName = node["Cue Name"].as<std::string>();

    if(node["Cue Number"])
        cue.CueNumber = { node["Cue Number"].as<float>() };

    if(node["Sound File Path"]) {
        cue.SoundFilePath = node["Sound File Path"].as<std::filesystem::path>();
    }
        

    return true;
}


// ====================================================================
// Cue Wrapper
// ====================================================================

Node convert<CueWrapper>::encode(const CueWrapper& cueWrapper) {

    Node node;

    node["Cue Type"] = static_cast<int>(cueWrapper.Type);
    node["Data"] = cueWrapper.CuePtr.get();

    return node;

}

bool convert<CueWrapper>::decode(const Node& node, CueWrapper& cueWrapper) {

    if(node["Cue Type"]) {
        cueWrapper.Type = static_cast<CueType>(node["Cue Type"].as<int>());
    }

    if(node["Data"]) {
        switch(cueWrapper.Type) {

        case CueType::CueTypeSound:
            cueWrapper.CuePtr = std::make_shared<SoundCue>(node["Data"].as<SoundCue>());
            break;
        case CueType::CueTypeEmpty:
        default:
            cueWrapper.CuePtr = std::make_shared<Cue>(node["Data"].as<Cue>());
            break;

        }
    }   

    return true;

}

// ====================================================================
// CueList
// ====================================================================
Node convert<CueList>::encode(const CueList& cueList) {
    Node node;

    std::vector<CueWrapper> cueWrappers;

    for(std::shared_ptr<Cue> sharedPtr : cueList.m_Registry) {
        // Type Determination
        CueType cueType = sharedPtr->IdentifyCueType();

        cueWrappers.push_back(CueWrapper{ cueType, sharedPtr });
    }

    node["Cues"] = cueWrappers;
    node["Cue List Order"] = cueList.m_CueListOrder;

    return node;
}

bool convert<CueList>::decode(const Node& node, CueList& cueList) {

    if(node["Cue List Order"])
        cueList.m_CueListOrder = node["Cue List Order"].as<std::vector<uint32_t>>();

    if(node["Cues"]) {
        for(auto wrapperNode : node["Cues"]) {
            auto cueWrapper = wrapperNode.as<CueWrapper>();

            std::shared_ptr<Cue> cue;

            // * Update here to add cue types *
            switch(cueWrapper.Type) {
                
            case CueType::CueTypeSound:
                cue = std::dynamic_pointer_cast<SoundCue>(cueWrapper.CuePtr);
                break;
            case CueType::CueTypeEmpty:
            default:
                cue = cueWrapper.CuePtr;
                break;
            }

            cueList.m_Registry.push_back(cue);
        }
    }

    cueList.UpdateCueCache();

    return true;
}

// ====================================================================
// Project
// ====================================================================
Node convert<Project>::encode(const Project& project) {
    Node node;

    node["Cue List"] = project.m_CueList;

    return node;
}

bool convert<Project>::decode(const Node& node, Project& project) {

    if(node["Cue List"])
        project.m_CueList = node["Cue List"].as<CueList>();

    return true;
}