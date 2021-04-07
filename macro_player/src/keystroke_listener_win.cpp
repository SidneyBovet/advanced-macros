#include "keycodes_win.hpp"
#include "macro_player/keystroke_listener.hpp"
#include "macro_player/logging.hpp"

#include <Windows.h>

#include <stdexcept>
#include <string>
#include <vector>

namespace macro_player::keystroke_listener
{
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
            case WM_CREATE:
            {
                // register interest in raw data
                RAWINPUTDEVICE rid;
                rid.usUsagePage = 0x01;                          // HID_USAGE_PAGE_GENERIC
                rid.usUsage = 0x06;                              // HID_USAGE_GENERIC_KEYBOARD
                rid.dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;  // receive system wide keystrokes
                rid.hwndTarget = hWnd;

                if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
                {
                    spdlog::error("Failed to register for raw input data: {}", GetLastError());
                }

                break;
            }
            case WM_INPUT:
            {
                spdlog::warn("Got an input message from the window procedure");
                break;
            }
            case WM_CLOSE:
                spdlog::warn("Received close event");
                PostQuitMessage(0);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
        }

        return NULL;
    }

    class KeystrokeListener::impl
    {
    private:
        void install_hook()
        {
            // Get our module handle
            if (!GetModuleHandleEx(NULL, NULL, &m_hinst))
            {
                spdlog::error("Failed to get module handle: {}", GetLastError());
            }

            // Create the window and register its procedure
            WNDCLASS wc = { 0 };
            wc.lpfnWndProc = WndProc;
            wc.hInstance = m_hinst;
            wc.lpszClassName = "advanced_macros";
            RegisterClass(&wc);
            m_windowHandle = CreateWindow(wc.lpszClassName, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, m_hinst, NULL);
        }

        HWND m_windowHandle = NULL;
        HINSTANCE m_hinst = NULL;

        keypress_callback m_callback;

    public:
        impl()
        {
            KeycodesWindows::prepare_reversed_map();
            install_hook();
        }

        ~impl() = default;

        void register_key_callback(const keypress_callback &c)
        {
            m_callback = c;
        }

        void process_one_message()
        {
            spdlog::debug("Getting message");

            MSG message;
            BOOL ret = GetMessage(&message, m_windowHandle, WM_INPUT, WM_INPUT);
            if (ret == 0)
            {
                // WM_QUIT
                spdlog::warn("Received quit event");
                exit(0);
            }

            switch (message.message)
            {
                case WM_INPUT:
                {
                    // Get required buffer size
                    UINT dwSize = 0;
                    if (GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER))
                        == -1)
                    {
                        spdlog::error("Error getting input buffer size: {}", GetLastError());
                        break;
                    }

                    // Allocate and fill buffer
                    LPBYTE bytes_buffer = new BYTE[dwSize];
                    if (bytes_buffer == NULL)
                    {
                        spdlog::error("Could not create byte buffer");
                        break;
                    }

                    if (GetRawInputData((HRAWINPUT)message.lParam,
                                        RID_INPUT,
                                        bytes_buffer,
                                        &dwSize,
                                        sizeof(RAWINPUTHEADER))
                        != dwSize)
                    {
                        spdlog::error("Error getting input buffer: {}", GetLastError());
                        delete[] bytes_buffer;
                        break;
                    }

                    // Grab virtual key code from buffer
                    RAWINPUT *raw = (RAWINPUT *)bytes_buffer;
                    DWORD type = raw->header.dwType;
                    RAWKEYBOARD kbd = raw->data.keyboard;
                    delete[] bytes_buffer;

                    if (type == RIM_TYPEKEYBOARD)
                    {
                        // React to key down event only
                        if (kbd.Message == WM_KEYDOWN || kbd.Message == WM_SYSKEYDOWN)
                        {
                            spdlog::debug("Pressed: {:#x} {:#x} {:#x} {:#x}",
                                          kbd.MakeCode,
                                          kbd.Flags,
                                          kbd.VKey,
                                          kbd.ExtraInformation);

                            // Windows gives us only VK_SHIFT or VK_CONTROL, instead of their LR variant
                            if (kbd.VKey == VK_SHIFT)
                            {
                                // Scan code for right shift key
                                if (kbd.MakeCode == 0x36)
                                    kbd.VKey = VK_RSHIFT;
                                else
                                    kbd.VKey = VK_LSHIFT;
                            }
                            else if (kbd.VKey == VK_CONTROL)
                            {
                                // E0 is set for right control
                                if ((kbd.Flags & RI_KEY_E0) != 0)
                                    kbd.VKey = VK_RCONTROL;
                                else
                                    kbd.VKey = VK_LCONTROL;
                            }
                            else if (kbd.VKey == VK_MENU)
                            {
                                // E0 is set for right alt
                                if ((kbd.Flags & RI_KEY_E0) != 0)
                                    kbd.VKey = VK_RMENU;
                                else
                                    kbd.VKey = VK_LMENU;
                            }

                            // Translate to qmk code and pass to callback
                            const auto &&qmk_code = KeycodesWindows::win_code_to_keycode(kbd.VKey);
                            if (qmk_code.empty())
                            {
                                spdlog::warn("Received unknown virtual key code {:#x}", kbd.VKey);
                            }
                            else if (m_callback)
                            {
                                m_callback(qmk_code);
                            }
                        }
                    }
                    else
                    {
                        spdlog::warn("Received a non-keyboard raw input, though we are only subscribed to keyboards");
                    }
                    break;
                }
                default:
                    spdlog::warn("Unexpected message type received: {}", message.message);
                    break;
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
