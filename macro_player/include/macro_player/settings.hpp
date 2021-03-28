#pragma once

#include "actions.hpp"

#include <fstream>
#include <unordered_map>

namespace macro_player::settings
{
    struct settings
    {
        std::unordered_map<actions::Keycode, std::shared_ptr<actions::Action>> actions;
    };

    class Settings
    {
    public:
        void load_settings(std::istream &input);
        const settings &get_settings();

    private:
        settings m_settings;
    };
}