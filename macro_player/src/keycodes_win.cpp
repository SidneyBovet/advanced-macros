#include "keycodes_win.hpp"

#include <macro_player/actions.hpp>

#include <Windows.h>

#include <WinUser.h>

#include <cassert>
#include <cstdint>
#include <unordered_map>


namespace macro_player
{
#pragma warning(push)
#pragma warning(disable : 4244)
    const std::unordered_map<actions::Keycode, uint16_t> KeycodesWindows::s_qmk_to_win {
        // virtual codes for 'A-Z' and '0-9' correspond to the ascii char code
        { "KC_A", 'A' },
        { "KC_B", 'B' },
        { "KC_C", 'C' },
        { "KC_D", 'D' },
        { "KC_E", 'E' },
        { "KC_F", 'F' },
        { "KC_G", 'G' },
        { "KC_H", 'H' },
        { "KC_I", 'I' },
        { "KC_J", 'J' },
        { "KC_K", 'K' },
        { "KC_L", 'L' },
        { "KC_M", 'M' },
        { "KC_N", 'N' },
        { "KC_O", 'O' },
        { "KC_P", 'P' },
        { "KC_Q", 'Q' },
        { "KC_R", 'R' },
        { "KC_S", 'S' },
        { "KC_T", 'T' },
        { "KC_U", 'U' },
        { "KC_V", 'V' },
        { "KC_W", 'W' },
        { "KC_X", 'X' },
        { "KC_Y", 'Y' },
        { "KC_Z", 'Z' },
        { "KC_1", '1' },
        { "KC_2", '2' },
        { "KC_3", '3' },
        { "KC_4", '4' },
        { "KC_5", '5' },
        { "KC_6", '6' },
        { "KC_7", '7' },
        { "KC_8", '8' },
        { "KC_9", '9' },
        { "KC_0", '0' },

        // These have been mapped semi-automatically
        { "KC_ENTER", VK_RETURN },
        { "KC_ESCAPE", VK_ESCAPE },
        { "KC_BSPACE", VK_BACK },
        { "KC_TAB", VK_TAB },
        { "KC_SPACE", VK_SPACE },
        { "KC_MINUS", VK_OEM_MINUS },
        { "KC_EQUAL", VK_OEM_NEC_EQUAL },
        { "KC_LBRACKET", VK_OEM_4 },
        { "KC_RBRACKET", VK_OEM_6 },
        { "KC_BSLASH", VK_OEM_5 },
        { "KC_NONUS_HASH", NOT_SUPPORTED },
        { "KC_SCOLON", VK_OEM_1 },
        { "KC_QUOTE", VK_OEM_7 },
        { "KC_GRAVE", VK_OEM_3 },
        { "KC_COMMA", VK_OEM_COMMA },
        { "KC_DOT", VK_OEM_PERIOD },
        { "KC_SLASH", VK_OEM_2 },
        { "KC_CAPSLOCK", VK_CAPITAL },
        { "KC_F1", VK_F1 },
        { "KC_F2", VK_F2 },
        { "KC_F3", VK_F3 },
        { "KC_F4", VK_F4 },
        { "KC_F5", VK_F5 },
        { "KC_F6", VK_F6 },
        { "KC_F7", VK_F7 },
        { "KC_F8", VK_F8 },
        { "KC_F9", VK_F9 },
        { "KC_F10", VK_F10 },
        { "KC_F11", VK_F11 },
        { "KC_F12", VK_F12 },
        { "KC_PSCREEN", VK_SNAPSHOT },
        { "KC_SCROLLLOCK", NOT_SUPPORTED },
        { "KC_PAUSE", NOT_SUPPORTED },
        { "KC_INSERT", VK_INSERT },
        { "KC_HOME", VK_HOME },
        { "KC_PGUP", VK_PRIOR },
        { "KC_DELETE", VK_DELETE },
        { "KC_END", VK_END },
        { "KC_PGDOWN", VK_PRIOR },
        { "KC_RIGHT", VK_RIGHT },
        { "KC_LEFT", VK_LEFT },
        { "KC_DOWN", VK_DOWN },
        { "KC_UP", VK_UP },
        { "KC_NUMLOCK", VK_NUMLOCK },
        { "KC_KP_SLASH", NOT_SUPPORTED },
        { "KC_KP_ASTERISK", NOT_SUPPORTED },
        { "KC_KP_MINUS", NOT_SUPPORTED },
        { "KC_KP_PLUS", NOT_SUPPORTED },
        { "KC_KP_ENTER", NOT_SUPPORTED },
        { "KC_KP_1", NOT_SUPPORTED },
        { "KC_KP_2", NOT_SUPPORTED },
        { "KC_KP_3", NOT_SUPPORTED },
        { "KC_KP_4", NOT_SUPPORTED },
        { "KC_KP_5", NOT_SUPPORTED },
        { "KC_KP_6", NOT_SUPPORTED },
        { "KC_KP_7", NOT_SUPPORTED },
        { "KC_KP_8", NOT_SUPPORTED },
        { "KC_KP_9", NOT_SUPPORTED },
        { "KC_KP_0", NOT_SUPPORTED },
        { "KC_KP_DOT", NOT_SUPPORTED },
        { "KC_NONUS_BSLASH", NOT_SUPPORTED },
        { "KC_APPLICATION", NOT_SUPPORTED },
        { "KC_POWER", NOT_SUPPORTED },
        { "KC_KP_EQUAL", NOT_SUPPORTED },
        { "KC_F13", VK_F13 },
        { "KC_F14", VK_F14 },
        { "KC_F15", VK_F15 },
        { "KC_F16", VK_F16 },
        { "KC_F17", VK_F17 },
        { "KC_F18", VK_F18 },
        { "KC_F19", VK_F19 },
        { "KC_F20", VK_F20 },
        { "KC_F21", VK_F21 },
        { "KC_F22", VK_F22 },
        { "KC_F23", VK_F23 },
        { "KC_F24", VK_F24 },
        { "KC_EXECUTE", VK_EXECUTE },
        { "KC_HELP", NOT_SUPPORTED },
        { "KC_MENU", NOT_SUPPORTED },
        { "KC_SELECT", VK_SELECT },
        { "KC_STOP", NOT_SUPPORTED },
        { "KC_AGAIN", NOT_SUPPORTED },
        { "KC_UNDO", NOT_SUPPORTED },
        { "KC_CUT", NOT_SUPPORTED },
        { "KC_COPY", NOT_SUPPORTED },
        { "KC_PASTE", NOT_SUPPORTED },
        { "KC_FIND", NOT_SUPPORTED },
        { "KC__MUTE", NOT_SUPPORTED },
        { "KC__VOLUP", NOT_SUPPORTED },
        { "KC__VOLDOWN", NOT_SUPPORTED },
        { "KC_LOCKING_CAPS", NOT_SUPPORTED },
        { "KC_LOCKING_NUM", NOT_SUPPORTED },
        { "KC_LOCKING_SCROLL", NOT_SUPPORTED },
        { "KC_KP_COMMA", NOT_SUPPORTED },
        { "KC_KP_EQUAL_AS400", 0 },
        { "KC_INT1", NOT_SUPPORTED },
        { "KC_INT2", NOT_SUPPORTED },
        { "KC_INT3", NOT_SUPPORTED },
        { "KC_INT4", NOT_SUPPORTED },
        { "KC_INT5", NOT_SUPPORTED },
        { "KC_INT6", NOT_SUPPORTED },
        { "KC_INT7", NOT_SUPPORTED },
        { "KC_INT8", NOT_SUPPORTED },
        { "KC_INT9", NOT_SUPPORTED },
        { "KC_LANG1", NOT_SUPPORTED },
        { "KC_LANG2", NOT_SUPPORTED },
        { "KC_LANG3", NOT_SUPPORTED },
        { "KC_LANG4", NOT_SUPPORTED },
        { "KC_LANG5", NOT_SUPPORTED },
        { "KC_LANG6", NOT_SUPPORTED },
        { "KC_LANG7", NOT_SUPPORTED },
        { "KC_LANG8", NOT_SUPPORTED },
        { "KC_LANG9", NOT_SUPPORTED },
        { "KC_ALT_ERASE", NOT_SUPPORTED },
        { "KC_SYSREQ", NOT_SUPPORTED },
        { "KC_CANCEL", NOT_SUPPORTED },
        { "KC_CLEAR", VK_OEM_CLEAR },
        { "KC_PRIOR", NOT_SUPPORTED },
        { "KC_RETURN", NOT_SUPPORTED },
        { "KC_SEPARATOR", NOT_SUPPORTED },
        { "KC_OUT", NOT_SUPPORTED },
        { "KC_OPER", NOT_SUPPORTED },
        { "KC_CLEAR_AGAIN", NOT_SUPPORTED },
        { "KC_CRSEL", NOT_SUPPORTED },
        { "KC_EXSEL", NOT_SUPPORTED },

        /* Modifiers */
        { "KC_LCTRL", VK_LCONTROL },
        { "KC_LSHIFT", VK_LSHIFT },
        { "KC_LALT", VK_LMENU },
        { "KC_LGUI", VK_LWIN },
        { "KC_RCTRL", VK_RCONTROL },
        { "KC_RSHIFT", VK_RSHIFT },
        { "KC_RALT", VK_RMENU },
        { "KC_RGUI", VK_RWIN },

        /* Generic Desktop Page (0x01) */
        { "KC_SYSTEM_POWER", NOT_SUPPORTED },
        { "KC_SYSTEM_SLEEP", VK_SLEEP },
        { "KC_SYSTEM_WAKE", NOT_SUPPORTED },

        /* Consumer Page (0x0C) */
        { "KC_AUDIO_MUTE", VK_VOLUME_MUTE },
        { "KC_AUDIO_VOL_UP", VK_VOLUME_UP },
        { "KC_AUDIO_VOL_DOWN", VK_VOLUME_DOWN },
        { "KC_MEDIA_NEXT_TRACK", VK_MEDIA_NEXT_TRACK },
        { "KC_MEDIA_PREV_TRACK", VK_MEDIA_PREV_TRACK },
        { "KC_MEDIA_STOP", VK_MEDIA_STOP },
        { "KC_MEDIA_PLAY_PAUSE", APPCOMMAND_MEDIA_PLAY_PAUSE },
        { "KC_MEDIA_SELECT", VK_LAUNCH_MEDIA_SELECT },
        { "KC_MEDIA_EJECT", NOT_SUPPORTED },
        { "KC_MAIL", VK_LAUNCH_MAIL },
        { "KC_CALCULATOR", NOT_SUPPORTED },
        { "KC_MY_COMPUTER", NOT_SUPPORTED },
        { "KC_WWW_SEARCH", VK_BROWSER_SEARCH },
        { "KC_WWW_HOME", VK_BROWSER_HOME },
        { "KC_WWW_BACK", VK_BROWSER_BACK },
        { "KC_WWW_FORWARD", VK_BROWSER_FORWARD },
        { "KC_WWW_STOP", VK_BROWSER_STOP },
        { "KC_WWW_REFRESH", VK_BROWSER_REFRESH },
        { "KC_WWW_FAVORITES", VK_BROWSER_FAVORITES },
        { "KC_MEDIA_FAST_FORWARD", VK_BROWSER_FORWARD },
        { "KC_MEDIA_REWIND", NOT_SUPPORTED },
        { "KC_BRIGHTNESS_UP", NOT_SUPPORTED },
        { "KC_BRIGHTNESS_DOWN", NOT_SUPPORTED }
    };
#pragma warning(pop)

    std::unordered_map<uint16_t, actions::Keycode> KeycodesWindows::s_win_to_qmk;

    void KeycodesWindows::prepare_reversed_map()
    {
        // Do it only once
        if (!s_win_to_qmk.empty())
        {
            return;
        }

        for (const auto &key_value : s_qmk_to_win)
        {
            s_win_to_qmk[key_value.second] = key_value.first;
        }
    }

    // std::unique_ptr<std::unordered_map<actions::Keycode, uint16_t>> s_qmk_to_win;
}
