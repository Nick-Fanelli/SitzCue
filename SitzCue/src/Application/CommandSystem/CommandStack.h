#pragma once

#include "sitzcuepch.h"

#include "Command.h"

namespace SitzCue {

    class CommandStack {

    public:
        CommandStack() = default;

        template<typename T, typename... Args>
        static Command* ExecuteCommand(Args&&... args) {

            SITZCUE_PROFILE_FUNCTION();

            Command* command = new T(std::forward<Args>(args)...);
            command->Execute();

            s_UndoCommands.push_back(command);

            if(s_UndoCommands.size() > CommandStackSize) {
                delete (*s_UndoCommands.begin());
                s_UndoCommands.erase(s_UndoCommands.begin());
            }

            s_RedoCommands.clear();

            return command;

        }

        static void Undo();
        static void Redo();

    public:
        static constexpr int CommandStackSize = 200;

    private:
        static inline std::vector<Command*> s_UndoCommands;
        static inline std::vector<Command*> s_RedoCommands;

    };
}