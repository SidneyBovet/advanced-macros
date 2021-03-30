#pragma once

#include "actions.hpp"

#include <iostream>
#include <memory>

namespace macro_player::settings
{
    class Settings
    {
    public:
        Settings();
        ~Settings();

        Settings(const Settings &) = delete;
        Settings &operator=(const Settings &) = delete;
        Settings(Settings &&) = delete;
        Settings &operator=(Settings &&) = delete;

        void load_settings(std::istream &input);
        const std::shared_ptr<actions::Action> get_action(const actions::Keycode &trigger) const;

    private:
        class impl;
        std::unique_ptr<impl> m_pimpl;
    };
}