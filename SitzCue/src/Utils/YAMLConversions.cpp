#include "YAMLConversions.h"

#include "Project/Cue.h"
#include "Project/Project.h"

using namespace SitzCue;
using namespace YAML;

// ====================================================================
// Cue
// ====================================================================
Node convert<Cue*>::encode(const Cue* cue) {
    Node node;

    node["UUID"] = static_cast<uint32_t>(cue->UUID);
    node["CueName"] = cue->CueName;

    if(cue->CueNumber.has_value())
        node["CueNumber"] = cue->CueNumber.value();

    return node;
}

bool convert<Cue>::decode(const Node& node, Cue& cue) {

    if(node["UUID"])
        cue.UUID = UUID(node["UUID"].as<uint32_t>());

    if(node["CueName"])
        cue.CueName = node["CueName"].as<std::string>();

    if(node["CueNumber"])
        cue.CueNumber = { node["CueNumber"].as<float>() };

    return true;
}

// ====================================================================
// CueList
// ====================================================================
Node convert<CueList>::encode(const CueList& cueList) {
    Node node;

    std::vector<Cue*> cuePointers;
    for(std::shared_ptr<Cue> sharedPtr : cueList.m_Registry) {
        cuePointers.push_back(sharedPtr.get());
    }

    node["Registry"] = cuePointers;
    node["CueListOrder"] = cueList.m_CueListOrder;

    return node;
}

bool convert<CueList>::decode(const Node& node, CueList& cueList) {

    if(node["CueListOrder"])
        cueList.m_CueListOrder = node["CueListOrder"].as<std::vector<uint32_t>>();

    if(node["Registry"]) {
        for(auto cue : node["Registry"]) {
            cueList.m_Registry.push_back(std::make_shared<Cue>(cue.as<Cue>()));
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

    project.m_CueList = node["CueList"].as<CueList>();

    return true;
}