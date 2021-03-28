#include "macro_player/settings.hpp"

#include <nlohmann/json.hpp>

namespace macro_player::settings
{
    void Settings::load_settings()
    {
        // Grab the file, read it
    }

    const settings &Settings::get_settings()
    {
        return m_settings;
    }
}
