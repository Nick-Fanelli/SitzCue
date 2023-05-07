#pragma once

#include "sitzcuepch.h"

#include "Command.h"

namespace SitzCue {

    class CommandStack {

    public:
        CommandStack() = default;

        static Command* ExecuteCommand(Command* command);

        static void Undo();
        static void Redo();

    public:
        static constexpr int CommandStackSize = 200;

    private:
        static inline std::vector<Command*> s_UndoCommands;
        static inline std::vector<Command*> s_RedoCommands;

    };
}