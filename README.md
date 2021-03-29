# Advanced Macros Player

## Work in progress

This project is being made.

## Concept

This is a library and executable to play advanced macros based on keyboard events. The main idea is to use any configurable input device (e.g. QMK keyboard, macro pad, Logitech gaming device) and map its buttons to key F13 through F24. This software can then pick up these key presses and perform the following actions:

* Play an arbitrary sequence of key presses, such as `Ctrl+Shift+Esc`
* Start a program, like `code -n`
* Any combination of the two options above, with some delay between them

## Usage

The configuration is done through a JSON file next to the app. Most of the QMK key codes (not aliases) are supported, see the [full QMK list](https://beta.docs.qmk.fm/using-qmk/simple-keycodes/keycodes).

### Example configuration to input `Win+D` on `F13`:

```json
{
    "KC_F13": ["KC_LGUI", "KC_D"]
}
```
