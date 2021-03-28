#include "hook.hpp"

static HWND s_mainWindowsHandle = NULL;

BOOL WINAPI DllMain([[maybe_unused]] HINSTANCE hinstDLL,  // handle to DLL module
                    [[maybe_unused]] DWORD fdwReason,     // reason for calling function
                    [[maybe_unused]] LPVOID lpReserved)   // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // Initialize once for each new process.
            // Return FALSE to fail DLL load.
            break;

        case DLL_THREAD_ATTACH:
            // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
            // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
            // Perform any necessary cleanup.
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

void register_callback(HWND windowHandle)
{
    s_mainWindowsHandle = windowHandle;
}

LRESULT CALLBACK KeyboardProc(_In_ int code, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    // skip if the hook procedure asks us to
    if (code < 0)
    {
        return CallNextHookEx(NULL, code, wParam, lParam);
    }

    if (code == HC_ACTION && wParam == WM_KEYDOWN)
    {
        // we just got a key stroke
        PostMessage(s_mainWindowsHandle, WM_USER, NULL, lParam);
    }

    return CallNextHookEx(NULL, code, wParam, lParam);
}
