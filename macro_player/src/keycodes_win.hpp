#pragma once

#include <macro_player/actions.hpp>

#include <cstdint>
#include <stdexcept>
#include <unordered_map>

#define NOT_SUPPORTED 0

namespace macro_player
{
    class KeycodesWindows
    {
    private:
        static const std::unordered_map<actions::Keycode, uint16_t> s_qmk_to_win;
        static std::unordered_map<uint16_t, actions::Keycode> s_win_to_qmk; // built dynamically

        public:
        // Returns a Windows Virtual-Key code from a qmk keycode
        inline static uint16_t keycode_to_win_code(const actions::Keycode &kc)
        {
            const auto &&found = s_qmk_to_win.find(kc);

            if (found == s_qmk_to_win.end())
            {
                throw std::runtime_error("Unknown keycode " + kc);
            }

            if (found->second == NOT_SUPPORTED)
            {
                throw std::runtime_error("Unsupported keycode " + kc);
            }

            return found->second;
        }

        // Returns a qmk keycode from a Windows Virtual-Key code
        inline static actions::Keycode win_code_to_keycode(uint16_t vk) noexcept
        {
            const auto &&found = s_win_to_qmk.find(vk);

            if (found == s_win_to_qmk.end())
            {
                return actions::Keycode();
            }

            return found->second;
        }

        // This should be run once on startup, to prepare the inverse mapping 's_win_to_qmk'
        static void prepare_reversed_map();
    };
}
