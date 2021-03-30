#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "macro_player/actions.hpp"
#include "macro_player/keystroke_emulator.hpp"
#include "macro_player/keystroke_listener.hpp"

#include <chrono>
#include <thread>

using namespace ::testing;
using namespace macro_player::keystroke_emulator;
using namespace macro_player::keystroke_listener;
using namespace macro_player::actions;

TEST(keypress_emulator_test, left_shift)
{
    Keycode key = "KC_LSHIFT";
    MockFunction<void(Keycode)> mock_callback;
    EXPECT_CALL(mock_callback, Call(key));
    KeystrokeListener listener;
    listener.register_key_callback(mock_callback.AsStdFunction());

    // press key in some time
    std::thread emulator_thread([&] {
        KeystrokeEmulator emulator;
        KeystrokeSequence keystrokes;
        keystrokes.actions.push_back(key);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        emulator.simulate(keystrokes);
    });

    listener.process_one_message();

    emulator_thread.join();
}

TEST(keypress_emulator_test, right_shift)
{
    Keycode key = "KC_RSHIFT";
    MockFunction<void(Keycode)> mock_callback;
    EXPECT_CALL(mock_callback, Call(key));
    KeystrokeListener listener;
    listener.register_key_callback(mock_callback.AsStdFunction());

    // press key in some time
    std::thread emulator_thread([&] {
        KeystrokeEmulator emulator;
        KeystrokeSequence keystrokes;
        keystrokes.actions.push_back(key);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        emulator.simulate(keystrokes);
    });

    listener.process_one_message();

    emulator_thread.join();
}
