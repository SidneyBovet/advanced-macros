#include <macro_player/keystroke_emulator.hpp>
#include <macro_player/keystroke_listener.hpp>
#include <macro_player/settings.hpp>
#include <macro_player/logging.hpp>

#include <Windows.h>

#include <chrono>
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

        macro_player::logging::setup_logging();
        spdlog::get("global")->debug("Started");

        macro_player::settings::Settings settings;
        settings.load_settings();
        spdlog::get("global")->info("Settings loaded");

        // macro_player::keystroke_listener::KeystrokeListener keyListener;
        // keyListener.process_one_message();

        macro_player::keystroke_emulator::KeystrokeEmulator keyEmulator;

        macro_player::actions::KeystrokeSequence keystrokes;
        keystrokes.actions.push_back("KC_LGUI");
        keystrokes.actions.push_back("KC_R");
        keyEmulator.simulate(keystrokes);
    }
    catch (const std::exception &e)
    {
        MessageBox(NULL, e.what(), TEXT("Macro Player error"), MB_ICONERROR);
    }

    return 0;
}
