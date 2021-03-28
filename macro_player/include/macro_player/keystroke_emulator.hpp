#pragma once

#include <macro_player/actions.hpp>

#include <memory>

namespace macro_player::keystroke_emulator
{
    class KeystrokeEmulator
    {
    public:
        KeystrokeEmulator();
        ~KeystrokeEmulator();

        KeystrokeEmulator(const KeystrokeEmulator &) = delete;
        KeystrokeEmulator &operator=(const KeystrokeEmulator &) = delete;
        KeystrokeEmulator(KeystrokeEmulator &&) = delete;
        KeystrokeEmulator &operator=(KeystrokeEmulator &&) = delete;

        // Simulates the given keystrokes, releasing them in a "first pressed, last released" order
        void simulate(const actions::KeystrokeSequence &keystrokes);

    private:
        class impl;
        std::unique_ptr<impl> m_pimpl;
    };
}