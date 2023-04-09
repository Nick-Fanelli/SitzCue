#pragma once

#include "sitzcuepch.h"

#include "Command.h"

namespace SitzCue {

    class CommandStack {

    public:
        CommandStack() = default;

        template<typename T, typename... Args>
        static Command* ExecuteCommand(Args&&... args) {

            Command* command = new T(std::forward<Args>(args)...);
            command->Execute();

            s_PreviousCommands.push_back(command);

            return command;

        }

    public:
        static constexpr int CommandStackSize = 100;

    private:
        static inline std::vector<Command*> s_PreviousCommands;
        static inline int s_PreviousCommandIndex = 0;

    };
}