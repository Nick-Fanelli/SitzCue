#include "Command.h"

using namespace SitzCue;

// ================================================================================================
// CreateCueCommand
// ================================================================================================
void CreateCueCommand::Execute() {
    
}

void CreateCueCommand::Undo() {
    
}

// ================================================================================================
// ChangeCueNameCommand
// ================================================================================================
void ChangeCueNameCommand::Execute() {
    
    m_Target = m_NewValue;

}

void ChangeCueNameCommand::Undo() {
    
    m_Target = m_PreviousValue;

}