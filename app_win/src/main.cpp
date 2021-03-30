#include <macro_player/actions_executor.hpp>
#include <macro_player/keystroke_emulator.hpp>
#include <macro_player/keystroke_listener.hpp>
#include <macro_player/logging.hpp>
#include <macro_player/process_launcher.hpp>
#include <macro_player/settings.hpp>

#include <Windows.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

int WINAPI WinMain([[maybe_unused]] _In_ HINSTANCE hInstance,
                   [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
                   [[maybe_unused]] _In_ LPSTR lpCmdLine,
                   [[maybe_unused]] _In_ int nShowCmd)
{
    try
    {
        SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_APPLICATION_DIR | LOAD_LIBRARY_SEARCH_SYSTEM32);

        macro_player::Logging::setup_logging();
        spdlog::debug("Started");

        auto settings = std::make_shared<macro_player::settings::Settings>();
        std::ifstream settingsFile("settings.json");
        settings->load_settings(settingsFile);

        macro_player::actions::ActionsExecutor actionsExecutor(settings);

        macro_player::keystroke_listener::KeystrokeListener keyListener;

        keyListener.register_key_callback([&](auto key) {
            actionsExecutor.execute_action(key);
        });

        while (true)
        {
            keyListener.process_one_message();
        }
    }
    catch (const std::exception &e)
    {
        MessageBox(NULL, e.what(), TEXT("Macro Player error"), MB_ICONERROR);
    }

    return 0;
}
