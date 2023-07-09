#include "Command.h"

using namespace SitzQ;

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
        m_CreatedCue = m_CueList.CreateCueAt(m_CueType, m_FollowingCueLocation.value());
    } else {
        m_CreatedCue = m_CueList.CreateCueAt(m_CueType, 0);
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
    // TODO: Implement
}

// ================================================================================================
// ChangeSoundFilepathCommand
// ================================================================================================
void ChangeSoundFilepathCommand::Execute() {
    auto cue = m_CueList.GetCue(m_TargetCue);

    if(cue->IdentifyCueType() == CueType::CueTypeSound) {
        SoundCue* cuePtr = dynamic_cast<SoundCue*>(cue.get());
        m_OldFilepath = cuePtr->GetSoundFilePath();
        cuePtr->SetSoundFilePath(m_NewFilepath);
    } else {
        Log::Error("Could not adapt cue type to SoundCue in ChangeFilepathCommand");
    }
}

void ChangeSoundFilepathCommand::Undo() {
    auto cue = m_CueList.GetCue(m_TargetCue);

    if(cue->IdentifyCueType() == CueType::CueTypeSound) {
        SoundCue* cuePtr = dynamic_cast<SoundCue*>(cue.get());
        cuePtr->SetSoundFilePath(m_OldFilepath);
    } else {
        Log::Error("Could not adapt cue type to SoundCue in ChangeFilepathCommand");
    }
}