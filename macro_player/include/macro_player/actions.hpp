#pragma once

#include <string>
#include <vector>
#include <memory>

namespace macro_player::actions
{
    // Note: since this comes from JSON, we use camelCase

    class Action
    {
    public:
        virtual ~Action() {};
        virtual const char* get_name() const = 0;
        // TODO: an action should be able to execute itself?
    };

    template <typename T>
    class Sequence : public Action
    {
    public:
        std::vector<T> actions;
        uint32_t msBetweenActions = 10;

        const char *get_name() const override
        {
            return "Sequence";
        }
    };

    class ProcessLaunch : public Action
    {
    public:
        std::string commandLine;

        const char *get_name() const override
        {
            return "ProcessLaunch";
        }
    };

    using Keycode = std::string;

    using KeystrokeSequence = Sequence<Keycode>;
    using ActionSequence = Sequence<std::shared_ptr<Action>>;
}