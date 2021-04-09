#include "macro_player/settings.hpp"

#include "actions_generated.h"
#include "flatbuffers/idl.h"
#include "flatbuffers/util.h"
#include "macro_player/logging.hpp"

#include <fstream>
#include <stdexcept>
#include <unordered_map>

namespace macro_player::settings
{
    class Settings::impl
    {
    private:
        std::unordered_map<actions::Keycode, std::shared_ptr<actions::Action>> m_actions_map;

        std::shared_ptr<actions::KeystrokeSequence> extract_kb_sequence(const schema::Keystrokes *ks_fb)
        {
            assert(ks_fb != nullptr);

            if (ks_fb->keys()->size() == 0)
            {
                return nullptr;
            }

            auto keystroke_action = std::make_shared<actions::KeystrokeSequence>();

            for (const auto *fb_string : *ks_fb->keys())
            {
                keystroke_action->actions.push_back(fb_string->c_str());
            }

            return keystroke_action;
        }

        std::shared_ptr<actions::ProcessLaunch> extract_launch_cmd(const schema::Launch *cmd_fb)
        {
            assert(cmd_fb != nullptr);

            if (cmd_fb->command()->size() == 0)
            {
                return nullptr;
            }

            auto launch_action = std::make_shared<actions::ProcessLaunch>();
            launch_action->commandLine = cmd_fb->command()->c_str();
            return launch_action;
        }

        std::shared_ptr<actions::ActionSequence> extract_actions_sequence(const schema::ActionsSequence *actions_seq_fb)
        {
            assert(actions_seq_fb != nullptr);

            if (actions_seq_fb->actions()->size() == 0)
            {
                return nullptr;
            }

            auto actions_sequence = std::make_shared<actions::ActionSequence>();

            for (const auto *action_fb : *actions_seq_fb->actions())
            {
                const auto action = extract_action(action_fb);
                if (action != nullptr)
                {
                    actions_sequence->actions.push_back(action);
                }
            }

            actions_sequence->msBetweenActions = actions_seq_fb->msDelay();

            return actions_sequence;
        }

        std::shared_ptr<actions::Action> extract_action(const schema::Action *action)
        {
            if (action == nullptr)
            {
                return nullptr;
            }

            std::shared_ptr<actions::Action> new_action;

            auto action_type = action->action_type();
            if (action_type == schema::ConcreteAction_Keystrokes)
            {
                const auto *as_keystrokes = action->action_as_Keystrokes();
                new_action = extract_kb_sequence(as_keystrokes);
            }
            else if (action_type == schema::ConcreteAction_Launch)
            {
                const auto *as_launch = action->action_as_Launch();
                new_action = extract_launch_cmd(as_launch);
            }
            else if (action_type == schema::ConcreteAction_ActionsSequence)
            {
                const auto *as_actions_seq = action->action_as_ActionsSequence();
                new_action = extract_actions_sequence(as_actions_seq);
            }
            else if (action_type == schema::ConcreteAction_NONE)
            {
                return nullptr;
            }
            else
            {
                spdlog::error("Unsupported action type {}", action_type);
                return nullptr;
            }

            if (new_action != nullptr && action->name() != nullptr)
            {
                new_action->name = action->name()->c_str();
            }

            return new_action;
        }

        // Generates and logs a sample setting json
        // To be hones this was more for development, when testing flatbuffers
        void propose_empty_settings()
        {
            flatbuffers::FlatBufferBuilder builder;

            // launch command on F13
            auto command = builder.CreateString("winver");
            auto process_launch = schema::CreateLaunch(builder, command);
            auto launch_trigger = builder.CreateString("KC_F13");
            auto launch_name = builder.CreateString("Show windows version");
            auto launch_action = schema::CreateAction(builder,
                                                      launch_trigger,
                                                      launch_name,
                                                      schema::ConcreteAction_Launch,
                                                      process_launch.Union());

            // Win+D on F14
            std::vector<std::string> keycodes { "KC_LGUI", "KC_D" };
            auto keycodes_vector = builder.CreateVectorOfStrings(keycodes);
            auto keystrokes = schema::CreateKeystrokes(builder, keycodes_vector);
            auto keystrokes_trigger = builder.CreateString("KC_F14");
            auto keystrokes_name = builder.CreateString("Return to desktop");
            auto keystrokes_action = schema::CreateAction(builder,
                                                          keystrokes_trigger,
                                                          keystrokes_name,
                                                          schema::ConcreteAction_Keystrokes,
                                                          keystrokes.Union());

            // Sequence of the two before on F15
            std::vector<flatbuffers::Offset<schema::Action>> actions_sequence_vector;
            actions_sequence_vector.push_back(keystrokes_action);
            actions_sequence_vector.push_back(launch_action);

            auto actions_sequence =
                schema::CreateActionsSequence(builder, builder.CreateVector(actions_sequence_vector));

            auto sequence_trigger = builder.CreateString("KC_F15");
            auto sequence_name = builder.CreateString("Return to desktop, then show windows version");
            auto actions_sequence_action = schema::CreateAction(builder,
                                                                sequence_trigger,
                                                                sequence_name,
                                                                schema::ConcreteAction_ActionsSequence,
                                                                actions_sequence.Union());

            // Put all actions together in a vector, and create the root Actions table
            std::vector<flatbuffers::Offset<schema::Action>> actions_std_vec;
            actions_std_vec.push_back(launch_action);
            actions_std_vec.push_back(keystrokes_action);
            actions_std_vec.push_back(actions_sequence_action);
            auto actions_vector = builder.CreateVector(actions_std_vec);
            auto actions = schema::CreateActions(builder, actions_vector);
            builder.Finish(actions);

            // Parse the schema and generate JSON
            flatbuffers::Parser parser;
            std::string schemafile;
            flatbuffers::LoadFile("actions.fbs", false, &schemafile);
            if (!parser.Parse(schemafile.c_str()))
            {
                spdlog::error("Couldn't read schema file: {}", parser.error_);
                return;
            }

            std::string jsongen;
            if (!GenerateText(parser, builder.GetBufferPointer(), &jsongen))
            {
                spdlog::error("Couldn't serialize data to JSON: {}", parser.error_);
                return;
            }

            spdlog::info("Settings not found, you may use this as a template:\n{}", jsongen);
        }

    public:
        void load_settings(std::istream &input)
        {
            spdlog::debug("Loading settings");

            try
            {
                const std::string content(std::istreambuf_iterator<char>(input), {});
                if (content.empty())
                {
                    propose_empty_settings();
                    exit(0);
                }

                // parse schema
                std::string schemafile;
                flatbuffers::LoadFile("actions.fbs", false, &schemafile);
                flatbuffers::Parser parser;
                if (!parser.Parse(schemafile.c_str()))
                {
                    spdlog::error("Error parsing schema: {}", parser.error_);
                    return;
                }

                // parse content
                if (!parser.Parse(content.c_str()))
                {
                    spdlog::error("Error parsing settings file: {}", parser.error_);
                    return;
                }

                // check that read content is valid
                auto verifyer = flatbuffers::Verifier(parser.builder_.GetBufferPointer(), parser.builder_.GetSize());
                if (!schema::VerifyActionsBuffer(verifyer))
                {
                    spdlog::error("Parsed actions is not valid");
                    return;
                }

                // populate actions map
                const auto *actions_fb = schema::GetActions(parser.builder_.GetBufferPointer())->actions();

                for (const auto action : *actions_fb)
                {
                    // check for missing trigger
                    if (action->trigger() == nullptr || action->trigger()->size() == 0)
                    {
                        spdlog::warn("Top-level action without a trigger, skipping");
                        continue;
                    }

                    // check for duplicates
                    if (m_actions_map.count(action->trigger()->c_str()) != 0)
                    {
                        spdlog::warn("We already have an action for {}, skipping additional definition",
                                     action->trigger()->c_str());
                        continue;
                    }

                    spdlog::info("Processing action for {}", action->trigger()->c_str());

                    // process the action
                    const auto new_action = extract_action(action);

                    // add that action to the map
                    if (new_action != nullptr)
                    {
                        m_actions_map[action->trigger()->c_str()] = new_action;
                    }
                    else
                    {
                        spdlog::warn("'{}' has no valid action, skipping", action->trigger()->c_str());
                    }
                }

                spdlog::info("Settings loaded");
            }
            catch (const std::exception &e)
            {
                spdlog::error("Error reading settings: {}", e.what());
            }
        }

        const std::shared_ptr<actions::Action> get_action(const actions::Keycode &trigger) const
        {
            auto found = m_actions_map.find(trigger);
            if (found == m_actions_map.end())
            {
                return nullptr;
            }

            return found->second;
        }
    };

    Settings::Settings() : m_pimpl(std::make_unique<impl>())
    {
    }

    Settings::~Settings() = default;

    void Settings::load_settings(std::istream &input)
    {
        m_pimpl->load_settings(input);
    }

    const std::shared_ptr<actions::Action> Settings::get_action(const actions::Keycode &trigger) const
    {
        return m_pimpl->get_action(trigger);
    }
}
