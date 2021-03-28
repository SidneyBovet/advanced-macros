#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "macro_player/settings.hpp"
#include "macro_player/actions.hpp"

#include <nlohmann/json.hpp>

#include <sstream>

using namespace ::testing;
using namespace macro_player::settings;
using namespace nlohmann;

TEST(settings_tests, broken_json)
{
    std::stringstream text_content("this is not a json string");

    Settings settings;
    ASSERT_NO_THROW(settings.load_settings(text_content));

    EXPECT_THAT(settings.get_settings().actions, IsEmpty());
}

TEST(settings_tests, string_field)
{
    json json_content = { "nope", "not working" };
    std::stringstream text_content(json_content.dump());

    Settings settings;
    ASSERT_NO_THROW(settings.load_settings(text_content));

    EXPECT_THAT(settings.get_settings().actions, IsEmpty());
}

TEST(settings_tests, number_field)
{
    json json_content = { "Test", 42 };
    std::stringstream text_content(json_content.dump());

    Settings settings;
    ASSERT_NO_THROW(settings.load_settings(text_content));

    EXPECT_THAT(settings.get_settings().actions, IsEmpty());
}

TEST(settings_tests, array_alone)
{
    json json_content = { "a", "b", "c" };
    std::stringstream text_content(json_content.dump());

    Settings settings;
    ASSERT_NO_THROW(settings.load_settings(text_content));

    EXPECT_THAT(settings.get_settings().actions, IsEmpty());
}

TEST(settings_tests, wrong_object_str)
{
    json json_content = { "KC_F13", { "KC_LGUI", "KC_D" } };
    std::stringstream text_content(json_content.dump());

    Settings settings;
    ASSERT_NO_THROW(settings.load_settings(text_content));

    EXPECT_THAT(settings.get_settings().actions, IsEmpty());
}

TEST(settings_tests, wrong_object_num)
{
    json json_content = { "KC_F13", { "KC_LGUI", 42 } };
    std::stringstream text_content(json_content.dump());

    Settings settings;
    ASSERT_NO_THROW(settings.load_settings(text_content));

    EXPECT_THAT(settings.get_settings().actions, IsEmpty());
}

TEST(settings_tests, wrong_object_array)
{
    json json_content = { "this", { "is", { "wr", "o", "ng" } } };
    std::stringstream text_content(json_content.dump());

    Settings settings;
    ASSERT_NO_THROW(settings.load_settings(text_content));

    EXPECT_THAT(settings.get_settings().actions, IsEmpty());
}

TEST(settings_tests, implicit_keystrokes)
{
    // "on F13, input Win+D"
    std::string json_content = R"({
  "KC_F13": [
    "KC_LGUI",
    "KC_D"
  ]
})";
    std::stringstream text_content(json_content);

    Settings settings;
    settings.load_settings(text_content);

    EXPECT_THAT(settings.get_settings().actions, Contains(Key("KC_F13")));
}

TEST(settings_tests, implicit_keystrokes_skip_invalid)
{
    // "on F13, input Win+???"
    std::string json_content = R"({
  "KC_F13": [
    "KC_LGUI",
    3.1415
  ]
})";
    std::stringstream text_content(json_content);

    Settings settings;
    settings.load_settings(text_content);

    ASSERT_THAT(settings.get_settings().actions, Contains(Key("KC_F13")));

    // convert action to keystroke sequence
    const auto action = settings.get_settings().actions.at("KC_F13");
    const auto *sequence = dynamic_cast<macro_player::actions::KeystrokeSequence *>(action.get());
    ASSERT_NE(sequence, nullptr);

    EXPECT_THAT(sequence->actions, ContainerEq(std::vector<std::string> { "KC_LGUI" }));
}

TEST(settings_tests, explicit_keystrokes)
{
    // "on F13, input Win+D"
    std::string json_content = R"({
  "KC_F13": {
    "keystrokes": [
      "KC_LGUI",
      "KC_D"
    ]
  }
})";
    std::stringstream text_content(json_content);

    Settings settings;
    settings.load_settings(text_content);

    EXPECT_THAT(settings.get_settings().actions, Contains(Key("KC_F13")));
}

TEST(settings_tests, explicit_keystrokes_with_timing)
{
    // "on F13, input Win+D, which 100ms in between"
    std::string json_content = R"({
  "KC_F13": {
    "keystrokes": {
      "actions": [
        "KC_LGUI",
        "KC_D"
      ],
      "msBetweenActions": 100
    }
  }
})";
    std::stringstream text_content(json_content);

    Settings settings;
    settings.load_settings(text_content);

    ASSERT_THAT(settings.get_settings().actions, Contains(Key("KC_F13")));

    // convert action to keystroke sequence
    const auto action = settings.get_settings().actions.at("KC_F13");
    const auto *sequence = dynamic_cast<macro_player::actions::KeystrokeSequence *>(action.get());
    ASSERT_NE(sequence, nullptr);

    // check that we have the right values
    EXPECT_THAT(sequence->actions, ContainerEq(std::vector<std::string> { "KC_LGUI", "KC_D" }));
    EXPECT_EQ(sequence->msBetweenActions, 100u);
}

TEST(settings_tests, explicit_keystrokes_with_wrong_timing)
{
    // "on F13, input Win+D, which 100ms in between"
    std::string json_content = R"({
  "KC_F13": {
    "keystrokes": {
      "actions": [
        "KC_LGUI",
        "KC_D"
      ],
      "msBetweenActions": "nope"
    }
  }
})";
    std::stringstream text_content(json_content);

    Settings settings;
    settings.load_settings(text_content);

    ASSERT_THAT(settings.get_settings().actions, Contains(Key("KC_F13")));

    // convert action to keystroke sequence
    const auto action = settings.get_settings().actions.at("KC_F13");
    const auto *sequence = dynamic_cast<macro_player::actions::KeystrokeSequence *>(action.get());
    ASSERT_NE(sequence, nullptr);

    // check that we have the right values
    EXPECT_THAT(sequence->actions, ContainerEq(std::vector<std::string> { "KC_LGUI", "KC_D" }));
    EXPECT_EQ(sequence->msBetweenActions, 10u); // default value
}

TEST(settings_tests, multiple_keystrokes)
{
    // "on F13, input Win+D, which 100ms in between"
    std::string json_content = R"({
  "KC_F13": {
    "keystrokes": {
      "actions": [
        "KC_LGUI",
        "KC_D"
      ],
      "msBetweenActions": 100
    }
  },
  "KC_F14": {
    "keystrokes": {
      "actions": [
        "KC_RGUI",
        "KC_D"
      ]
    }
  }
})";
    std::stringstream text_content(json_content);

    Settings settings;
    settings.load_settings(text_content);

    ASSERT_THAT(settings.get_settings().actions, SizeIs(2));
    EXPECT_THAT(settings.get_settings().actions, Contains(Key("KC_F13")));
    ASSERT_THAT(settings.get_settings().actions, Contains(Key("KC_F14")));

    // convert action to keystroke sequence
    const auto action = settings.get_settings().actions.at("KC_F14");
    const auto *sequence = dynamic_cast<macro_player::actions::KeystrokeSequence *>(action.get());
    ASSERT_NE(sequence, nullptr);

    // check that we have the right values
    EXPECT_THAT(sequence->actions, ContainerEq(std::vector<std::string> { "KC_RGUI", "KC_D" }));
    EXPECT_EQ(sequence->msBetweenActions, 10u);  // default value
}


TEST(settings_tests, multiple_keystrokes_with_error)
{
    // "on F13, input Win+D, which 100ms in between"
    std::string json_content = R"({
  "KC_F13": {
    "keystrokes": {
      "actions": [
        "KC_LGUI",
        "KC_D"
      ],
      "msBetweenActions": 100
    }
  },
  "KC_F14": {
    "unknown": {
      "thing": 42
    }
  }
})";
    std::stringstream text_content(json_content);

    Settings settings;
    settings.load_settings(text_content);

    ASSERT_THAT(settings.get_settings().actions, SizeIs(1));
    ASSERT_THAT(settings.get_settings().actions, Contains(Key("KC_F13")));

    // convert action to keystroke sequence
    const auto action = settings.get_settings().actions.at("KC_F13");
    const auto *sequence = dynamic_cast<macro_player::actions::KeystrokeSequence *>(action.get());
    ASSERT_NE(sequence, nullptr);

    // check that we have the right values
    EXPECT_THAT(sequence->actions, ContainerEq(std::vector<std::string> { "KC_LGUI", "KC_D" }));
    EXPECT_EQ(sequence->msBetweenActions, 100u);
}
