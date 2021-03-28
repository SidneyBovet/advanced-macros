#pragma once

#include <memory>

namespace macro_player::keystroke_listener
{
    class KeystrokeListener
    {
    public:
        KeystrokeListener();
        ~KeystrokeListener();

        KeystrokeListener(const KeystrokeListener &) = delete;
        KeystrokeListener &operator=(const KeystrokeListener &) = delete;
        KeystrokeListener(KeystrokeListener &&) = delete;
        KeystrokeListener &operator=(KeystrokeListener &&) = delete;

        void process_one_message();

    private:
        class impl;
        std::unique_ptr<impl> m_pimpl;
    };
}