#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "macro_player/actions.hpp"
#include "macro_player/settings.hpp"

#include <nlohmann/json.hpp>

#include <sstream>

using namespace ::testing;
using namespace macro_player::settings;
using namespace nlohmann;

TEST(settings_tests, malformed_data)
{
    std::string json_content = R"({
  trigger: "test_action",
  command: {
    command: "my_program"
  }
})";
    std::stringstream text_content(json_content);

    Settings settings;
    ASSERT_NO_THROW(settings.load_settings(text_content));
}

TEST(settings_tests, simple_actions)
{
    std::string json_content = R"({
  actions: [
    {
      trigger: "KC_F13",
      name: "Start command",
      action_type: "Launch",
      action: {
        command: "command"
      }
    },
    {
      trigger: "KC_F14",
      name: "Return to desktop",
      action_type: "Keystrokes",
      action: {
        keys: [
          "KC_LGUI",
          "KC_D"
        ]
      }
    }
  ]
})";
    std::stringstream text_content(json_content);

    Settings settings;
    settings.load_settings(text_content);

    ASSERT_NE(settings.get_action("KC_F13"), nullptr);

    // convert to keystroke sequence
    const auto action = settings.get_action("KC_F14");
    ASSERT_NE(action, nullptr);
    const auto *sequence = dynamic_cast<macro_player::actions::KeystrokeSequence *>(action.get());
    ASSERT_NE(sequence, nullptr);

    // check that we have the right values
    EXPECT_THAT(sequence->actions, ContainerEq(std::vector<std::string> { "KC_LGUI", "KC_D" }));
    EXPECT_EQ(sequence->name, "Return to desktop");
}

TEST(settings_tests, missing_trigger)
{
    std::string json_content = R"({
  actions: [
    {
      action_type: "Launch",
      action: {
        command: "command"
      }
    },
    {
      trigger: "KC_F14",
      action_type: "Keystrokes",
      action: {
        keys: [
          "KC_LGUI",
          "KC_D"
        ]
      }
    }
  ]
})";
    std::stringstream text_content(json_content);

    Settings settings;
    settings.load_settings(text_content);

    ASSERT_NE(settings.get_action("KC_F14"), nullptr);
}

TEST(settings_tests, missing_action)
{  // action will be null in fb
    std::string json_content = R"({
  actions: [
    {
      trigger: "KC_F13"
    }
  ]
})";
    std::stringstream text_content(json_content);

    Settings settings;
    ASSERT_NO_THROW(settings.load_settings(text_content));
}

TEST(settings_tests, missing_action_type)
{
    std::string json_content = R"({
  actions: [
    {
      trigger: "KC_F14",
      action: {
        keys: [
          "KC_LGUI",
          "KC_D"
        ],
        msDelay: 100
      }
    }
  ]
})";
    std::stringstream text_content(json_content);

    Settings settings;
    ASSERT_NO_THROW(settings.load_settings(text_content));
}

TEST(settings_tests, wrong_action_type)
{
    std::string json_content = R"({
  actions: [
    {
      trigger: "KC_F13",
      action_type: "Keystrokes",
      action: {
        command: "command"
      }
    }
  ]
})";
    std::stringstream text_content(json_content);

    Settings settings;
    ASSERT_NO_THROW(settings.load_settings(text_content));
}

TEST(settings_tests, extra_fields)
{  // e.g. text comments as fields
    std::string json_content = R"({
  actions: [
    {
      some_number: 42,
      trigger: "KC_F13",
      action_type: "Launch",
      action: {
        command: "command"
      }
    },
    {
      trigger: "KC_F14",
      action_type: "Keystrokes",
      comment: "this is a comment",
      action: {
        keys: [
          "KC_LGUI",
          "KC_D"
        ],
        msDelay: 100
      }
    }
  ]
})";
    std::stringstream text_content(json_content);

    Settings settings;
    ASSERT_NO_THROW(settings.load_settings(text_content));
}

TEST(settings_tests, with_json_comments)
{  // unauthorized by JSON standard, let's see
    std::string json_content = R"({
  actions: [
    {
      // json does not allow comments
      trigger: "KC_F13",
      action_type: "Launch",
      action: {
        command: "command"
      }
    },
    {
      trigger: "KC_F14",
      action: {
        keys: [
          "KC_LGUI",
          "KC_D"
        ],
        msDelay: 100
      }
    }
  ]
})";
    std::stringstream text_content(json_content);

    Settings settings;
    ASSERT_NO_THROW(settings.load_settings(text_content));
}

TEST(settings_tests, trigger_redefinition)
{
    std::string json_content = R"({
  actions: [
    {
      trigger: "KC_F13",
      action_type: "Launch",
      action: {
        command: "command"
      }
    },
    {
      trigger: "KC_F13",
      action_type: "Keystrokes",
      action: {
        keys: [
          "KC_LGUI",
          "KC_D"
        ]
      }
    }
  ]
})";
    std::stringstream text_content(json_content);

    Settings settings;
    settings.load_settings(text_content);

    // convert to command
    const auto action = settings.get_action("KC_F13");
    ASSERT_NE(action, nullptr);
    const auto *sequence = dynamic_cast<macro_player::actions::ProcessLaunch *>(action.get());
    ASSERT_NE(sequence, nullptr);

    // check that we have the right values
    EXPECT_EQ(sequence->commandLine, "command");
}

TEST(settings_tests, action_sequence)
{
    std::string json_content = R"({
  actions: [
    {
      trigger: "KC_F15",
      action_type: "ActionsSequence",
      action: {
        actions: [
          {
            action_type: "Keystrokes",
            action: {
              keys: [
                "KC_LGUI",
                "KC_D"
              ]
            }
          },
          {
            action_type: "Launch",
            action: {
              command: "command"
            }
          }
        ],
        msDelay: 100
      }
    }
  ]
})";
    std::stringstream text_content(json_content);

    Settings settings;
    settings.load_settings(text_content);

    const auto action = settings.get_action("KC_F15");
    ASSERT_NE(action, nullptr);

    // convert to action sequence
    const auto *sequence = dynamic_cast<macro_player::actions::ActionSequence *>(action.get());
    ASSERT_NE(sequence, nullptr);

    // check that we have the right values
    EXPECT_EQ(sequence->actions.size(), 2);
    EXPECT_EQ(sequence->msBetweenActions, 100u);
}

TEST(settings_tests, action_sequence_with_missing_action)
{  // action will be null in fb
    std::string json_content = R"({
  actions: [
    {
      trigger: "KC_F15",
      action_type: "ActionsSequence",
      action: {
        actions: [
          {
            action_type: "Keystrokes",
            action: {
              keys: [
                "KC_LGUI",
                "KC_D"
              ]
            }
          },
          {
          }
        ]
      }
    }
  ]
})";
    std::stringstream text_content(json_content);

    Settings settings;
    settings.load_settings(text_content);

    const auto action = settings.get_action("KC_F15");
    ASSERT_NE(action, nullptr);

    // convert to action sequence
    const auto *sequence = dynamic_cast<macro_player::actions::ActionSequence *>(action.get());
    ASSERT_NE(sequence, nullptr);

    // check that we have the right values
    EXPECT_EQ(sequence->actions.size(), 1);
    auto first_action = dynamic_cast<macro_player::actions::KeystrokeSequence *>(sequence->actions[0].get());
    EXPECT_NE(first_action, nullptr);
    EXPECT_THAT(first_action->actions, Contains("KC_LGUI"));
    EXPECT_THAT(first_action->actions, Contains("KC_D"));
}
