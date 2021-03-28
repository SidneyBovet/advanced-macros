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

    class ProcessLaunch : public Action
    {
        std::string commandLine;
    };

    using Keystroke = uint16_t;

    class KeystrokeSequence : public Action
    {
        std::vector<Keystroke> keystrokes;
        uint32_t delayBetweenStrokes;
    };
}