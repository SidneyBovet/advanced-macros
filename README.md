# Advanced Macros Player

## Concept

This application lets you play advanced macros based on keyboard events. The main idea is to use any configurable input device (e.g. QMK keyboard, macro pad, Logitech gaming device) and map its buttons to key F13 through F24, so we don't interfere with anything else on the system. This software can then pick up these key presses and perform the following actions:

* Play an arbitrary sequence of key presses, such as <kbd>Ctrl</kbd><kbd>Shift</kbd><kbd>Esc</kbd>
* Start a program, like `code -n`
* Any combination of the options above, with some delay between them

## Usage & Configuration

The configuration is done through a JSON file next to the app. By default the program picks up a file named `config.json`, but you can specify the config file using the `--config=[file]` option. Running with `--help` shows you the available options.

A config file is a list of actions:

```json
{
  "actions": [
      {
        "trigger": "KC_ESCAPE",
        "name": "A description",
        "actionType": "<see below>",
        "action": { }
      }
  ]
}
```

The `trigger` field is a QMK keycode. Most of the QMK key codes (not aliases) are supported, see the [full QMK list](https://beta.docs.qmk.fm/using-qmk/simple-keycodes/keycodes). The `name` field is optional and only serves as a description of what the action does, like a note for your future self.

The `actionType` can be `Keystrokes`, `Launch`, or `ActionSequence`.

The `action`'s content depends on the chosen type:

### Keystrokes

```json
{
  "trigger": "KC_A",
  "action_type": "Keystrokes",
  "action": {
    "keys": [
      "KC_LGUI",
      "KC_D"
    ]
  }
}
```

This will input <kbd>Win</kbd><kbd>D</kbd> when <kbd>A</kbd> is pressed (you probably don't want to do that).

### Launch

```json
{
  "trigger": "KC_SLASH",
  "name": "Start firefox",
  "action_type": "Launch",
  "action": {
    "command": "C:\\Program Files\\Mozilla Firefox\\firefox.exe"
  }
}
```

This will open Firefox when pressing <kbd>/</kbd>.

### ActionSequence

This is the most complex action type, but it is quite easy to understand when you see it as a series of the two actions above.

```json
{
  "trigger": "KC_F15",
  "name": "Perform multiple actions",
  "action_type": "ActionsSequence",
  "action": {
    "actions": [
      {
        "name": "First action",
        "action_type": "...",
        "action": { }
      },
      {
        "name": "Second action",
        "action_type": "...",
        "action": { }
      }
    ],
    "msDelay": 1000
  }
}
```

Here we will perform a series of actions after <kbd>F15</kbd> is pressed. Note the absence of the `trigger` field in the sub-actions, as we do not need this field. Such an action sequence can be assigned a delay, in milliseconds. In this example the first action will be executed, then one second will pass before the second action is played.

A more complete example can be seen [in this example file](example_config.json), and the exact data format is described [here](schema/src/actions.fbs).

## Supported platforms

 :green_circle: Windows
 :red_circle: Mac OS
 :red_circle: Linux

Currently only Windows is supported. The code is made in a way that platform support may be extended in the future. Any contribution is welcome!

The classes handling platform-dependent features are identified by an `_win` postfix, like [keystroke_emulator_win.cpp](macro_player/src/keystroke_emulator_win.cpp).

## Building the code

[CMake](https://cmake.org/) is required to generate the project, and leverages [CPM](https://github.com/cpm-cmake/CPM.cmake) to pull the few libraries needed by this project.

Other than that it is a pretty standard CMake project, so use this to generate the project in a folder called `build`:

```bash
cmake.exe -B build -G "Visual Studio 16 2019" -A x64
```

And to build an optimized version from the command line, use:

```bash
cmake.exe --build build --config Release
```
