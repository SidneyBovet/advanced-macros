#include "macro_player/settings.hpp"

#include <nlohmann/json.hpp>
#include "macro_player/logging.hpp"

#include <fstream>
#include <stdexcept>

namespace macro_player::settings
{
    std::shared_ptr<actions::KeystrokeSequence> get_sequence(const nlohmann::json &seq_array)
    {
        assert(seq_array.is_array());

        auto kc_seq = std::make_shared < actions::KeystrokeSequence>();
        for (const auto &kc : seq_array)
        {
            if (!kc.is_string())
            {
                spdlog::error("Value '{}' is not a string, skipping", kc.dump());
                continue;
            }

            const auto code_string = kc.get<std::string>();
            if (code_string.rfind("KC_") != 0)
            {
                spdlog::error("Value '{}' doesn't look like a key code, skipping", code_string);
                continue;
            }

            kc_seq->actions.push_back(code_string);
        }

        return kc_seq;
    }

    settings parse_json(const nlohmann::json &content)
    {
        settings parsed;

        auto dump = content.dump();

        if (!content.is_object())
        {
            spdlog::error("Settings do not represent a JSON object, skipping");
            return parsed;
        }

        for (auto &[key, value] : content.items())
        {
            if (value.is_array())
            {
                // implicit keystroke sequence
                parsed.actions[key] = get_sequence(value);
            }
            else if (value.is_string())
            {
                // implicit command
            }
            else if (!value.is_object())
            {
                // value should be an object (we will parse it as an Action)
                spdlog::error("Key {} has unrecognized setting, skipping", key);
                continue;
            }
            else if (value.count("keystrokes") != 0 && value["keystrokes"].is_array())
            {
                // explicit keystroke sequence without timing
                parsed.actions[key] = get_sequence(value["keystrokes"]);
            }
            else if (value.count("keystrokes") != 0 && value["keystrokes"].is_object())
            {
                // explicit keystroke sequence possibly with timing
                nlohmann::json keystrokes_obj = value["keystrokes"];

                if (keystrokes_obj.count("actions") == 0 || !keystrokes_obj["actions"].is_array())
                {
                    spdlog::error("keystrokes for {} is not an array, skipping", key);
                    continue;
                }

                auto ks_seq = get_sequence(keystrokes_obj["actions"]);

                if (keystrokes_obj["msBetweenActions"].is_number())
                {
                    ks_seq->msBetweenActions = keystrokes_obj["msBetweenActions"];
                }

                parsed.actions[key] = ks_seq;
            }
        }

        return parsed;
    }

    void Settings::load_settings(std::istream &input)
    {
        try
        {
            spdlog::debug("Loading settings");
            spdlog::info("Loading settings");
            nlohmann::json read_json = nlohmann::json::parse(input);
            m_settings = parse_json(read_json);
            spdlog::info("Settings loaded");
        }
        catch (const nlohmann::json::exception& e)
        {
            spdlog::error("Error reading settings: {}", e.what());
        }

    }

    const settings &Settings::get_settings()
    {
        return m_settings;
    }
}
