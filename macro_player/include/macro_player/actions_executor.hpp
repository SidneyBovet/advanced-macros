#pragma once

#include "macro_player/actions.hpp"
#include "macro_player/keystroke_emulator.hpp"
#include "macro_player/process_launcher.hpp"
#include "macro_player/settings.hpp"

#include <memory>

namespace macro_player::actions
{
    using Keycode = std::string;

    class ActionsExecutor
    {
    public:
        ActionsExecutor(std::shared_ptr<settings::Settings> settings);
        ~ActionsExecutor() = default;

        void execute_action(const Keycode &k);

    private:
        void execute_action(const std::shared_ptr<Action> &action);

        std::shared_ptr<settings::Settings> m_settings;
        keystroke_emulator::KeystrokeEmulator m_emulator;
        process_launch::ProcessLauncher m_launcher;
    };
}