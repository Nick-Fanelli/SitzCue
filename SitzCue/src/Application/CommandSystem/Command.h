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


    template<typename T>
    class UpdateGenericDataCommand : public Command {

    public:
        UpdateGenericDataCommand(T& ref, const T& previousValue, const T& newValue)
            : m_Ref(ref), m_PreviousValue(previousValue), m_NewValue(newValue) {}

        ~UpdateGenericDataCommand() = default;

        void Execute() override {
            m_Ref = m_NewValue;
        }

        void Undo() override {
            m_Ref = m_PreviousValue;
        }

    private:   
        T& m_Ref;
        T m_PreviousValue;
        T m_NewValue;

    };


    class CreateNewCueCommand : public Command {

    public:
        CreateNewCueCommand(CueList& cueList)
            : m_CueList(cueList) {}

        ~CreateNewCueCommand() = default;

        void Execute() override {
            m_CreatedCue = m_CueList.CreateCue();
        }

        void Undo() override {
            m_CueList.DeleteCue(m_CreatedCue);
        }

    private:   
        CueList& m_CueList;
        std::shared_ptr<Cue> m_CreatedCue;

    };


    class DeleteCueCommand : public Command {

    public:
        DeleteCueCommand(CueList& cueList, std::shared_ptr<Cue> cue)
            : m_CueList(cueList), m_Cue(cue) {}

        DeleteCueCommand(CueList& cueList, UUID uuid) : m_CueList(cueList) {
            m_Cue = cueList.GetCue(uuid);
        }

        ~DeleteCueCommand() = default;

        void Execute() override {
            m_CuePosition = m_CueList.GetListPositionOfCue(m_Cue);
            m_CueList.DeleteCue(m_Cue);
        }

        void Undo() override {
            m_CueList.ReinstateCue(m_Cue, m_CuePosition);
        }

    private:   
        CueList& m_CueList;
        std::shared_ptr<Cue> m_Cue;

        uint32_t m_CuePosition;

    };

}