#include "CommandStack.h"

using namespace SitzCue;

void CommandStack::Undo() {

    if(!s_UndoCommands.empty()) {
        auto* lastCommand = s_UndoCommands.back();
        s_UndoCommands.pop_back();

        s_RedoCommands.push_back(lastCommand);
        lastCommand->Undo();
    }

}

void CommandStack::Redo() {

    if(!s_RedoCommands.empty()) {
        auto* lastCommand = s_RedoCommands.back();
        s_RedoCommands.pop_back();

        s_UndoCommands.push_back(lastCommand);
        lastCommand->Execute();
    }

}