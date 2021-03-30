#pragma once

#include <memory>
#include <string>
#include <vector>

namespace macro_player::actions
{
    using Keycode = std::string;

    class Action
    {
    public:
        enum class ActionType
        {
            UnknownSequence,
            ProcessLaunch,
            KeystrokeSequence,
            ActionSequence
        };

        virtual ~Action() {};
        virtual ActionType get_type() const = 0;

        // TODO: should an action be able to execute itself?
    };

    template<typename T>
    class Sequence : public Action
    {
    public:
        std::vector<T> actions;

        ActionType get_type() const override
        {
            return ActionType::UnknownSequence;
        }
    };

    class ProcessLaunch : public Action
    {
    public:
        std::string commandLine;

        ActionType get_type() const override
        {
            return ActionType::ProcessLaunch;
        }
    };

    class KeystrokeSequence : public Sequence<Keycode>
    {
    public:
        ActionType get_type() const override
        {
            return ActionType::KeystrokeSequence;
        }
    };

    class ActionSequence : public Sequence<std::shared_ptr<Action>>
    {
    public:
        uint32_t msBetweenActions = 10;

        ActionType get_type() const override
        {
            return ActionType::ActionSequence;
        }
    };
}