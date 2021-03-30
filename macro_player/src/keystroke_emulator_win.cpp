
#include "keycodes_win.hpp"
#include "macro_player/keystroke_emulator.hpp"

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
    public:
        impl()
        {
        }

        ~impl()
        {
        }

        void simulate(const actions::KeystrokeSequence &keystrokes)
        {
            const size_t total_inputs = keystrokes.actions.size() * 2;

            // OutputDebugString("Sending 'Win-D'\r\n");
            std::vector<INPUT> inputs;

            // press each key in order
            for (const auto &keystroke : keystrokes.actions)
            {
                INPUT i;
                ZeroMemory(&i, sizeof(INPUT));
                i.type = INPUT_KEYBOARD;

                i.ki.wVk = KeycodesWindows::keycode_to_win_code(keystroke);

                inputs.push_back(i);
            }

            // release each key in reverse order
            for (auto it = keystrokes.actions.rbegin(); it != keystrokes.actions.rend(); ++it)
            {
                INPUT i;
                ZeroMemory(&i, sizeof(INPUT));
                i.type = INPUT_KEYBOARD;

                i.ki.wVk = KeycodesWindows::keycode_to_win_code(*it);
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
                OutputDebugString("SendInput failed\n");
                const auto error = HRESULT_FROM_WIN32(GetLastError());
                throw std::runtime_error("SendInput failed with error: " + std::to_string(error));
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
