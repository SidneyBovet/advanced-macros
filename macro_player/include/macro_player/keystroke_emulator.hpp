#pragma once

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

        void process_one_message();

    private:
        class impl;
        std::unique_ptr<impl> m_pimpl;
    };
}