#include "Command.h"

using namespace SitzCue;

// ================================================================================================
// BatchCommand
// ================================================================================================

BatchCommand::BatchCommand(const std::vector<Command*>& batchCommands) {
    for(Command* command : batchCommands)
        m_Commands.push_back(command);
}

BatchCommand::~BatchCommand() {
    for(auto* command : m_Commands)
        delete command;
}

void BatchCommand::Execute() {
    for(Command* command : m_Commands)
        command->Execute();
}

void BatchCommand::Undo() {
    for(auto it = m_Commands.rbegin(); it != m_Commands.rend(); ++it) {
        (*it)->Undo();
    }
}

void BatchCommand::PushBackCommand(Command* command) {
    m_Commands.push_back(command);
}

// ================================================================================================
// CreateNewCueCommand
// ================================================================================================

void CreateNewCueCommand::Execute() {

    if(m_FollowingCueLocation.has_value()) {
        m_CreatedCue = m_CueList.CreateCue(m_FollowingCueLocation.value());
    } else {
        m_CreatedCue = m_CueList.CreateCue(0);
    }
}

void CreateNewCueCommand::Undo() {
    m_CueList.DeleteCue(m_CreatedCue);
}

// ================================================================================================
// DeleteCueCommand
// ================================================================================================

void DeleteCueCommand::Execute() {
    m_CuePosition = m_CueList.GetListPositionOfCue(m_Cue);
    m_CueList.DeleteCue(m_Cue);
}

void DeleteCueCommand::Undo() {
    m_CueList.ReinstateCue(m_Cue, m_CuePosition);
}

// ================================================================================================
// MoveCueCommand
// ================================================================================================
void MoveCueCommand::Execute() {
    m_CueList.MoveCue(m_TargetCue, m_CueToFollow);
}

void MoveCueCommand::Undo() {

}