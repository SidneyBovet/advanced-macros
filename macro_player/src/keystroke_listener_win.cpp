#include "keystroke_listener.hpp"

#include <Windows.h>

#include <delayimp.h>

#include <hook.hpp>

#include <atomic>
#include <iostream>
#include <stdexcept>
#include <string>

namespace macro_player::keystroke_listener
{
    class KeystrokeListener::impl
    {
    private:
        void load_dll()
        {
            m_hinstDLL = LoadLibrary(TEXT("windows_hook.dll"));

            if (m_hinstDLL == 0)
                throw std::runtime_error("Couldn't load windows hook DLL");

            m_windowHandle = GetActiveWindow();
            register_callback(m_windowHandle);
        }

        void free_dll()
        {
            FreeLibrary(m_hinstDLL);
        }

        void install_hook()
        {
            HOOKPROC hkprcSysMsg;
            hkprcSysMsg = (HOOKPROC)GetProcAddress(m_hinstDLL, "KeyboardProc");

            if (hkprcSysMsg == NULL)
            {
                std::string message = "Couldn't load hook procedure, error: ";
                message += std::to_string(GetLastError());
                throw std::runtime_error(message);
            }

            m_hhookListener = SetWindowsHookEx(WH_KEYBOARD_LL, hkprcSysMsg, m_hinstDLL, 0);
        }

        void remove_hook()
        {
            UnhookWindowsHookEx(m_hhookListener);
        }

        HWND m_windowHandle = NULL;
        HINSTANCE m_hinstDLL = NULL;
        HHOOK m_hhookListener = NULL;

    public:
        impl()
        {
            load_dll();
            install_hook();
        }

        ~impl()
        {
            remove_hook();
            free_dll();
        }

        void process_one_message()
        {
            MSG message;
            BOOL ret = GetMessage(&message, m_windowHandle, WM_USER, WM_USER);
            if (ret == 0)
            {
                // WM_QUIT
                UnhookWindowsHookEx(m_hhookListener);
                exit(0);
            }
            else if (ret == -1)
            {
                // Error
                std::string message = "Couldn't get message, error: ";
                message += std::to_string(GetLastError());
                throw std::runtime_error(message);
            }
            else
            {
                // process message
                KBDLLHOOKSTRUCT keyboardEvent = *((KBDLLHOOKSTRUCT *)message.lParam);
                // TODO: forward to actions dispatcher
            }
        }
    };

    KeystrokeListener::KeystrokeListener() : m_pimpl {std::make_unique<impl>()}
    {
    }

    KeystrokeListener::~KeystrokeListener() = default;

    void KeystrokeListener::process_one_message()
    {
        m_pimpl->process_one_message();
    }
}
