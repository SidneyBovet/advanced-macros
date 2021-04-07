#include "keycodes_win.hpp"
#include "macro_player/keystroke_listener.hpp"
#include "macro_player/logging.hpp"

#include <Windows.h>

#include <hook.hpp>

#include <stdexcept>
#include <string>
#include <vector>

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

        keypress_callback m_callback;

    public:
        impl()
        {
            KeycodesWindows::prepare_reversed_map();

            load_dll();
            install_hook();
        }

        ~impl()
        {
            remove_hook();
            free_dll();
        }

        void register_key_callback(const keypress_callback &c)
        {
            m_callback = c;
        }

        void process_one_message()
        {
            MSG message;
            spdlog::debug("Getting message");
            BOOL ret = GetMessage(&message, m_windowHandle, WM_USER, WM_USER);
            if (ret == 0)
            {
                // WM_QUIT
                spdlog::warn("Received quit event");
                UnhookWindowsHookEx(m_hhookListener);
                exit(0);
            }
            else if (ret == -1)
            {
                // Error
                std::string what = "Couldn't get message, error: ";
                what += std::to_string(GetLastError());
                spdlog::error(what);
                throw std::runtime_error(what);
            }
            else
            {
                // process message
                KBDLLHOOKSTRUCT keyboardEvent = *((KBDLLHOOKSTRUCT *)message.lParam);

                spdlog::debug("Received vkCode {:#x}, scanCode {:#x}", keyboardEvent.vkCode, keyboardEvent.scanCode);

                // translate to qmk and pass to callback
                if (keyboardEvent.vkCode >= 65535)
                {
                    spdlog::info("vkCode bigger than 16 bits, ignoring", keyboardEvent.vkCode);
                    return;
                }


#pragma warning(push)
#pragma warning(disable : 4244) // already checked that vkCode < 65535
                const auto &&qmk_code = KeycodesWindows::win_code_to_keycode(keyboardEvent.vkCode);
#pragma warning(pop)
                if (!qmk_code.empty() && m_callback)
                {
                    m_callback(qmk_code);
                }
            }
        }
    };

    KeystrokeListener::KeystrokeListener() : m_pimpl { std::make_unique<impl>() }
    {
    }

    KeystrokeListener::~KeystrokeListener() = default;

    void KeystrokeListener::process_one_message()
    {
        m_pimpl->process_one_message();
    }

    void KeystrokeListener::register_key_callback(const keypress_callback &c)
    {
        m_pimpl->register_key_callback(c);
    }
}
