#pragma once

#include <string>
#include <vector>

namespace macro_player::actions
{
    class Action
    {
    public:
        virtual ~Action() {};
    };

    template <typename T>
    class Sequence : public Action
    {
    public:
        std::vector<T> actions;
        uint32_t delayBetweenActions;
    };

    class ProcessLaunch : public Action
    {
    public:
        std::string commandLine;
    };

    using Keycode = std::string;

    using KeystrokeSequence = Sequence<Keycode>;
    using ActionSequence = Sequence<Action>;
}