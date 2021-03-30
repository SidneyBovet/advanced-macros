#pragma once

#include "macro_player/actions.hpp"

#include <functional>
#include <memory>

namespace macro_player::keystroke_listener
{
    class KeystrokeListener
    {
    public:
        using keypress_callback = std::function<void(actions::Keycode)>;

        KeystrokeListener();
        ~KeystrokeListener();

        KeystrokeListener(const KeystrokeListener &) = delete;
        KeystrokeListener &operator=(const KeystrokeListener &) = delete;
        KeystrokeListener(KeystrokeListener &&) = delete;
        KeystrokeListener &operator=(KeystrokeListener &&) = delete;

        void register_key_callback(const keypress_callback &c);
        void process_one_message();

    private:
        class impl;
        std::unique_ptr<impl> m_pimpl;
    };
}