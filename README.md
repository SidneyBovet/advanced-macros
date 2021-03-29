# Advanced Macros Player

## Work in progress

This project is being made.

## Concept

This is a library and executable to play advanced macros based on keyboard events. The main idea is to use any configurable input device (e.g. QMK keyboard, macro pad, Logitech gaming device) and map its buttons to key F13 through F24. This software can then pick up these key presses and perform the following actions:

* Play an arbitrary sequence of key presses, such as <kbd>Ctrl</kbd><kbd>Shift</kbd><kbd>Esc</kbd>
* Start a program, like `code -n`
* Any combination of the options above, with some delay between them

## Usage

The configuration is done through a JSON file next to the app. Most of the QMK key codes (not aliases) are supported, see the [full QMK list](https://beta.docs.qmk.fm/using-qmk/simple-keycodes/keycodes).

### Example configuration to input <kbd>Win</kbd><kbd>D</kbd> on <kbd>F13</kbd>:



```json
{
    "KC_F13": ["KC_LGUI", "KC_D"]
}
```

## Supported platforms

 :green_circle: Windows
 :red_circle: Mac OS
 :red_circle: Linux

Currently only Windows is supported. The code is made in a way that platform support may be extended in the future. Any contribution is welcome!
