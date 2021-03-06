
#include "keycodes_win.hpp"
#include "macro_player/keystroke_emulator.hpp"
#include "macro_player/logging.hpp"

#include <Windows.h>

#include <WinUser.h>

#include <cassert>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace macro_player::keystroke_emulator
{
    class KeystrokeEmulator::impl
    {
    private:
        void set_input(INPUT &i, actions::Keycode kycode)
        {
            i.ki.wVk = KeycodesWindows::keycode_to_win_code(kycode);

            // We also need scan code and flags for L/R shift, alt, control
            switch (i.ki.wVk)
            {
                case VK_LSHIFT:
                    i.ki.wScan = 0x2a;
                    break;
                case VK_RSHIFT:
                    i.ki.wScan = 0x36;
                    break;
                case VK_LCONTROL:
                    break;
                case VK_RCONTROL:
                    i.ki.dwFlags |= RI_KEY_E0;
                    break;
                case VK_LMENU:
                    break;
                case VK_RMENU:
                    i.ki.dwFlags |= RI_KEY_E0;
                    break;
                default:
                    break;
            }
        }

    public:
        impl()
        {
        }

        ~impl()
        {
        }

        void simulate(const actions::KeystrokeSequence &keystrokes)
        {
            std::vector<INPUT> inputs;

            // press each key in order
            for (const auto &keystroke : keystrokes.actions)
            {
                INPUT i;
                ZeroMemory(&i, sizeof(INPUT));
                i.type = INPUT_KEYBOARD;

                set_input(i, keystroke);

                inputs.push_back(i);
            }

            // release each key in reverse order
            for (auto it = keystrokes.actions.rbegin(); it != keystrokes.actions.rend(); ++it)
            {
                INPUT i;
                ZeroMemory(&i, sizeof(INPUT));
                i.type = INPUT_KEYBOARD;

                set_input(i, *it);

                i.ki.dwFlags = KEYEVENTF_KEYUP;

                inputs.push_back(i);
            }

#pragma warning(push)
#pragma warning(disable : 4267)
            assert(inputs.size() < 0xFFFF);
            UINT uSent = SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
#pragma warning(pop)
            if (uSent != inputs.size())
            {
                const auto error = HRESULT_FROM_WIN32(GetLastError());
                spdlog::error("SendInput failed with error: {}", error);
            }
        }
    };

    KeystrokeEmulator::KeystrokeEmulator() : m_pimpl { std::make_unique<impl>() }
    {
    }

    KeystrokeEmulator::~KeystrokeEmulator() = default;

    void KeystrokeEmulator::simulate(const actions::KeystrokeSequence &keystrokes)
    {
        m_pimpl->simulate(keystrokes);
    }
}
