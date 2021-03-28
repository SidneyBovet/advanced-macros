#include <macro_player/keystroke_listener.hpp>
#include <macro_player/settings.hpp>

#include <Windows.h>

#include <iostream>
#include <thread>
#include <chrono>

int WINAPI WinMain([[maybe_unused]] _In_ HINSTANCE hInstance,
                   [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
                   [[maybe_unused]] _In_ LPSTR lpCmdLine,
                   [[maybe_unused]] _In_ int nShowCmd)
{
    try
    {
        SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_APPLICATION_DIR | LOAD_LIBRARY_SEARCH_SYSTEM32);

        macro_player::settings::Settings settings;
        settings.load_settings();

        macro_player::keystroke_listener::KeystrokeListener keyListener;
        keyListener.process_one_message();
    }
    catch (const std::exception &e)
    {
        MessageBox(NULL, e.what(), TEXT("Macro Player error"), MB_ICONERROR);
    }

    return 0;
}
