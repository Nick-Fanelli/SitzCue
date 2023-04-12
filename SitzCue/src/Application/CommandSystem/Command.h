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

    
    class UpdateStringCommand : public Command {

    public:
        UpdateStringCommand(std::string& ref, const std::string& previousValue, const std::string& newValue)
            : m_Ref(ref), m_PreviousValue(previousValue), m_NewValue(newValue) {}

        ~UpdateStringCommand() = default;

        void Execute() override {

            m_Ref = m_NewValue;

        }

        void Undo() override {

            m_Ref = m_PreviousValue;

        }

    private:    
        std::string& m_Ref;
        std::string m_PreviousValue;
        std::string m_NewValue;

    };


    class UpdateOptionalFloatCommand : public Command {

    public:
        UpdateOptionalFloatCommand(std::optional<float>& ref, const std::optional<float>& previousValue, const std::optional<float>& newValue) 
        : m_Ref(ref), m_PreviousValue(previousValue), m_NewValue(newValue) {}

        ~UpdateOptionalFloatCommand() = default;

        void Execute() override {

            m_Ref = m_NewValue;

        }

        void Undo() override {

            m_Ref = m_PreviousValue;

        }

    private:
        std::optional<float>& m_Ref;
        std::optional<float> m_PreviousValue;
        std::optional<float> m_NewValue;

    };

}