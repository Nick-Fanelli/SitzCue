#include "YAMLConversions.h"

#include "Project/Cue.h"
#include "Project/Project.h"

using namespace SitzCue;
using namespace YAML;

// ====================================================================
// Sound Cue
// ====================================================================


// ====================================================================
// Cue*
// ====================================================================
Node convert<Cue*>::encode(const Cue* constCue) {
    Node node;

    Cue* cue = const_cast<Cue*>(constCue);

    if(dynamic_cast<SoundCue*>(cue)) 
        node["Sound File"] = "Some File Here";
    
    node["UUID"] = static_cast<uint32_t>(cue->UUID);
    node["CueName"] = cue->CueName;

    if(cue->CueNumber.has_value())
        node["CueNumber"] = cue->CueNumber.value();

    return node;
}


// ====================================================================
// Cue
// ====================================================================
bool convert<Cue>::decode(const Node& node, Cue& cue) {

    if(node["UUID"])
        cue.UUID = UUID(node["UUID"].as<uint32_t>());

    if(node["CueName"])
        cue.CueName = node["CueName"].as<std::string>();

    if(node["CueNumber"])
        cue.CueNumber = { node["CueNumber"].as<float>() };

    return true;
}

bool convert<SoundCue>::decode(const Node& node, SoundCue& cue) {

    if(node["UUID"])
        cue.UUID = UUID(node["UUID"].as<uint32_t>());

    if(node["CueName"])
        cue.CueName = node["CueName"].as<std::string>();

    if(node["CueNumber"])
        cue.CueNumber = { node["CueNumber"].as<float>() };

    return true;
}


// ====================================================================
// Cue Wrapper
// ====================================================================

Node convert<CueWrapper>::encode(const CueWrapper& cueWrapper) {

    Node node;

    node["CueType"] = static_cast<int>(cueWrapper.Type);
    node["Data"] = cueWrapper.CuePtr.get();

    return node;

}

bool convert<CueWrapper>::decode(const Node& node, CueWrapper& cueWrapper) {

    if(node["CueType"]) {
        cueWrapper.Type = static_cast<CueType>(node["CueType"].as<int>());
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
    node["CueListOrder"] = cueList.m_CueListOrder;

    return node;
}

bool convert<CueList>::decode(const Node& node, CueList& cueList) {

    if(node["CueListOrder"])
        cueList.m_CueListOrder = node["CueListOrder"].as<std::vector<uint32_t>>();

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

    node["CueList"] = project.m_CueList;

    return node;
}

bool convert<Project>::decode(const Node& node, Project& project) {

    if(node["CueList"])
        project.m_CueList = node["CueList"].as<CueList>();

    return true;
}