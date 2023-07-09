#pragma once

#include "sitzqpch.h"

#include "Project/Cue.h"
#include "Project/CueList.h"

namespace SitzQ {

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


    class BatchCommand : public Command {

    public: 

        BatchCommand() = default;

        BatchCommand(const std::vector<Command*>& batchCommands);
        ~BatchCommand();

        void Execute() override;
        void Undo() override;

        void PushBackCommand(Command* command);

    private:

        std::vector<Command*> m_Commands;

    };

    class CreateNewCueCommand : public Command {

    public:
        CreateNewCueCommand(CueList& cueList, CueType cueType)
            : m_CueList(cueList), m_CueType(cueType), m_FollowingCueLocation({}) {}

        CreateNewCueCommand(CueList& cueList, CueType cueType, UUID followingCueLocation) 
            : m_CueList(cueList), m_CueType(cueType), m_FollowingCueLocation(followingCueLocation) {}

        ~CreateNewCueCommand() = default;

        void Execute() override;
        void Undo() override;

    private:   
        CueList& m_CueList;
        CueType m_CueType;
        std::shared_ptr<Cue> m_CreatedCue;
        std::optional<UUID> m_FollowingCueLocation;

    };


    class DeleteCueCommand : public Command {

    public:
        DeleteCueCommand(CueList& cueList, std::shared_ptr<Cue> cue)
            : m_CueList(cueList), m_Cue(cue) {}

        DeleteCueCommand(CueList& cueList, UUID uuid) : m_CueList(cueList) {
            m_Cue = cueList.GetCue(uuid);
        }

        ~DeleteCueCommand() = default;

        void Execute() override;
        void Undo() override;

    private:   
        CueList& m_CueList;
        std::shared_ptr<Cue> m_Cue;

        uint32_t m_CuePosition;

    };


    class MoveCueCommand : public Command {

    public:
        MoveCueCommand(CueList& cueList, UUID targetCue, UUID cueToFollow) 
            : m_CueList(cueList), m_TargetCue(targetCue), m_CueToFollow(cueToFollow) {}

        void Execute() override;
        void Undo() override;

    private:
        CueList& m_CueList;
        UUID m_TargetCue;
        UUID m_CueToFollow;

    };


    class ChangeSoundFilepathCommand : public Command {

    public:
        ChangeSoundFilepathCommand(CueList& cueList, UUID targetCue, const std::filesystem::path& newFilepath) 
            : m_CueList(cueList), m_TargetCue(targetCue), m_NewFilepath(newFilepath) {}

        void Execute() override;
        void Undo() override;

    private: 
        CueList& m_CueList;
        UUID m_TargetCue;
        std::filesystem::path m_NewFilepath;

        std::filesystem::path m_OldFilepath;

    };
}