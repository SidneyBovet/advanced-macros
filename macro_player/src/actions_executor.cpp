#include "macro_player/actions_executor.hpp"

#include "macro_player/logging.hpp"

#include <cassert>
#include <chrono>
#include <thread>

namespace macro_player::actions
{
    ActionsExecutor::ActionsExecutor(std::shared_ptr<settings::Settings> settings) : m_settings(std::move(settings))
    {
    }

    void ActionsExecutor::execute_action(const Keycode &keycode)
    {
        const auto action = m_settings->get_action(keycode);

        if (action == nullptr)
        {
            // no action for that key
            return;
        }

        spdlog::info("Executing action '{}' bound to {}", action->name, keycode);

        execute_action(action);
    }

    void ActionsExecutor::execute_action(const std::shared_ptr<Action> &action)
    {
        assert(action != nullptr);

        switch (action->get_type())
        {
            case Action::ActionType::KeystrokeSequence:
            {
                KeystrokeSequence *ks_action = dynamic_cast<KeystrokeSequence *>(action.get());
                assert(ks_action != nullptr);
                m_emulator.simulate(*ks_action);
                break;
            }
            case Action::ActionType::ProcessLaunch:
            {
                ProcessLaunch *pl_action = dynamic_cast<ProcessLaunch *>(action.get());
                assert(pl_action != nullptr);
                m_launcher.start_process_detached(*pl_action);
                break;
            }
            case Action::ActionType::ActionSequence:
            {
                auto seq_action = std::dynamic_pointer_cast<ActionSequence>(action);
                assert(seq_action != nullptr);
                for (const auto &sub_action : seq_action->actions)
                {
                    execute_action(sub_action);
                    std::this_thread::sleep_for(std::chrono::milliseconds(seq_action->msBetweenActions));
                }
                break;
            }
            default:
                spdlog::error("Unknown action type {}", action->get_type());
        }
    }
}