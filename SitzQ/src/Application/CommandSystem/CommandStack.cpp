#include "CommandStack.h"

using namespace SitzQ;

Command* CommandStack::ExecuteCommand(Command* command) {
    SITZCUE_PROFILE_FUNCTION();

    if(command == nullptr)
        return nullptr;

    command->Execute();


    s_UndoCommands.push_back(command);

    if(s_UndoCommands.size() > CommandStackSize) {
        delete (*s_UndoCommands.begin());
        s_UndoCommands.erase(s_UndoCommands.begin());
    }

    s_RedoCommands.clear();

    return command;
}

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