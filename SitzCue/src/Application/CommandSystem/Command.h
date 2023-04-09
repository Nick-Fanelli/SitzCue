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


    class UpdateStringCommand : public Command {

    public:

        UpdateStringCommand(Ref<std::string> target, const std::string& previousValue, const std::string& newValue) : m_Target(target), m_PreviousValue(previousValue), m_NewValue(newValue) {}
        ~UpdateStringCommand() = default;

        void Execute() override {
            m_Target.SetValueIfSafe(m_NewValue);
        }

        void Undo() override {
            m_Target.SetValueIfSafe(m_PreviousValue);
        }

    private:
        Ref<std::string> m_Target;
        std::string m_PreviousValue;
        std::string m_NewValue;

    };


    class UpdateFloatCommand : public Command {

    public:

        UpdateFloatCommand(Ref<float> target, float previousValue, float newValue) : m_Target(target), m_PreviousValue(previousValue), m_NewValue(newValue) {}
        ~UpdateFloatCommand() = default;

        void Execute() override {   
            m_Target.SetValueIfSafe(m_NewValue);
        }

        void Undo() override {
            m_Target.SetValueIfSafe(m_PreviousValue);
        }

    private:
        Ref<float> m_Target;
        float m_PreviousValue;
        float m_NewValue;

    };

}