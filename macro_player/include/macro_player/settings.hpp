#pragma once

#include "actions.hpp"

#include <unordered_map>

namespace macro_player::settings
{
    struct settings
    {
        std::unordered_map<actions::Keycode, actions::Action> actions;
    };

    class Settings
    {
    public:
        void load_settings();
        const settings &get_settings();

    private:
        settings m_settings;
    };
}