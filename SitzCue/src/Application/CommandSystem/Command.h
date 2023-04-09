#pragma once

#include "sitzcuepch.h"

#include "Project/Cue.h"

namespace SitzCue {

    class Command {

    public:
        Command() = default;
        virtual ~Command() {}

        virtual void Execute() = 0;
        virtual void Undo() = 0;

    };

    // ================================================================================================
    // CreateCueCommand
    // ====================================r============================================================

    class CreateCueCommand : public Command {

    public:

        CreateCueCommand(CueList& cueList) : m_CueList(cueList) {}
        ~CreateCueCommand() = default;

        void Execute() override;
        void Undo() override;

    private:
        CueList& m_CueList;
        UUID m_CreatedCueUUID;

    };

    // ================================================================================================
    // ChangeCueNameCommand
    // ================================================================================================

    class ChangeCueNameCommand : public Command {

    public:

        ChangeCueNameCommand(std::string& target, const std::string& previousValue, const std::string& newValue) : m_Target(target), m_PreviousValue(previousValue), m_NewValue(newValue) {}
        ~ChangeCueNameCommand() = default;

        void Execute() override;
        void Undo() override;

    private:
        std::string& m_Target;
        std::string m_PreviousValue;
        std::string m_NewValue;

    };

}