#include "args.hxx"

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
#include <string>
#include <thread>

int WINAPI WinMain([[maybe_unused]] _In_ HINSTANCE hInstance,
                   [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
                   [[maybe_unused]] _In_ LPSTR lpCmdLine,
                   [[maybe_unused]] _In_ int nShowCmd)
{
    SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_APPLICATION_DIR | LOAD_LIBRARY_SEARCH_SYSTEM32);

    // Argument parsing
    args::ArgumentParser arg_parser("Simulate key presses and launch processes on keyboard events.", "");
    args::HelpFlag arg_help(arg_parser, "help", "Display this help content", { 'h', "help" });
    args::Flag arg_verbose(arg_parser, "verbose", "Set verbose log output", { 'v', "verbose" });
    args::ValueFlag<std::string> arg_settings(arg_parser,
                                              "file",
                                              "Path to config JSON (defaults to './config.json')",
                                              { 'c', "config" });

    try
    {
        arg_parser.ParseCLI(__argc, __argv);
    }
    catch (args::Help)
    {
        MessageBox(NULL, arg_parser.Help().c_str(), TEXT("Macro Player help"), MB_ICONINFORMATION);
        return 0;
    }
    catch (args::Error &e)
    {
        std::ostringstream message;
        message << e.what() << std::endl << arg_parser;
        MessageBox(NULL, message.str().c_str(), TEXT("Macro Player error"), MB_ICONWARNING);
        return 1;
    }

    spdlog::level::level_enum log_level = spdlog::level::info;
    if (arg_verbose)
    {
        log_level = spdlog::level::debug;
    }

    std::string settings_file = "config.json";
    if (arg_settings)
    {
        settings_file = args::get(arg_settings);
    }

    // Set up and run
    try
    {
        macro_player::Logging::setup_logging(log_level);
        spdlog::debug("Started");

        auto settings = std::make_shared<macro_player::settings::Settings>();
        std::ifstream settingsFile(settings_file);
        settings->load_settings(settingsFile);

        macro_player::actions::ActionsExecutor actionsExecutor(settings);

        macro_player::keystroke_listener::KeystrokeListener keyListener;

        keyListener.register_key_callback([&](auto key) {
            actionsExecutor.execute_action(key);
        });

        spdlog::info("Start processing messages...");

        while (true)
        {
            // TODO: add a way to exit the application
            keyListener.process_one_message();
            spdlog::debug("Done processing one message");
        }
    }
    catch (const std::exception &e)
    {
        MessageBox(NULL, e.what(), TEXT("Macro Player error"), MB_ICONERROR);
    }

    return 0;
}
