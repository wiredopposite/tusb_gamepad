# tusb_gamepad
A library that can emulate gamepads for several platforms using TinyUSB. Currently it's only usuable with the Raspberry Pi RP2040 (Pico) and Espressif chips with built in USB (though it's only been tested on the ESP32S3).

## Supported platforms
- Original Xbox
- XInput
- PlayStation 3
- PlayStation Classic
- Nintendo Switch

## Requirements
You will need TinyUSB added to your project, tusb_gamepad implements all necessary TinyUSB callbacks for you. 

### Config file
You will need to add these lines to your tusb_config.h file:
```
#include "board_config.h"

#define MAX_GAMEPADS 1 // or more if wanted for DInput/PlayStation 3
```
And also download the TinyUSB dependancies for your board:
```
cd tinyusb // wherever you downloaded tinyusb
python tools/get_deps.py rp2040
```
While compiling, the library will automatically redefine some TinyUSB settings to make sure everything works properly with tusb_gamepad. To avoid getting redefine warnings from the compiler, you can just use a tusb_config.h file from one of the tusb_gamepad example projects listed below.

## Usage
This library is still in very eary development so usage is subject to change. 

### Template/example projects

I've made a couple template projects to demonstrate integrating and using this library in your project. Take note of the CMakeLists.txt files in each.

- ESP-IDF: 
    - https://github.com/wiredopposite/tusb_gamepad_example_esp-idf
- PICO-SDK: 
    - https://github.com/wiredopposite/tusb_gamepad_example_pico-sdk
    - https://github.com/wiredopposite/tusb_gamepad_ds4_to_xinput_pico-sdk (uses Pico-PIO-USB)

### Interacting with the gamepad object
To change the gamepad object's button, trigger, and joystick values or read rumble values:

If you're only using one gamepad (MAX_GAMEPADS = 1), pass 0 as the arg to gamepad(int idx):
```
#include "tusb_gamepad.h"

void update_gamepad()
{
    Gamepad* gp = gamepad(0);

    gp->reset_pad(gp);        // resets buttons, triggers, and joysticks

    // different way to enable buttons, which are a button mask
    gp->buttons.a   = 1;
    gp->buttons.x   = true;

    gp->triggers.l   = 0xFF;  // triggers are uint8_t
    gp->joysticks.ly = 30000; // joysticks are full range int16_t
}
```
Or alternatively you can just use the gamepad function directly
```
#include "tusb_gamepad.h"

void update_gamepad()
{
    gamepad(0)->buttons.a    = true; 
    gamepad(0)->triggers.l   = 0xFF; 
    gamepad(0)->joysticks.ly = 30000;
}
```
There is also built-in support for analog buttons (uint8) for DInput (PlayStation 3) and Original Xbox. To enable this functionality, you only need to do this once:
```
gamepad(0)->enable_analog_buttons = true;
```
The analog buttons will be enabled from that point onward, or until you set enable_analog_buttons to false.
```
#include "tusb_gamepad.h"

void update_analog_buttons()
{
    gamepad(0)->analog_buttons.a = 0xFF;
    gamepad(0)->analog_buttons.x = 0xFF / 2;
    gamepad(0)->analog_buttons.y = 0xFF / 4;
}
```

If MAX_GAMEPADS is set to greater than 1, idx for gamepad(int idx) should be between 0 and MAX_GAMEPADS-1, otherwise a null is returned
```
#include "tusb_gamepad.h"

void update_all_gamepads()
{
    for (int i = 0; i < MAX_GAMEPADS; i++)
    {
        Gamepad* gp = gamepad(i);

        if (gp != NULL)
        {
            gp->buttons.a    = true; 
            gp->triggers.l   = 0xFF; 
            gp->joysticks.ly = 30000;
        }
    }
}
```
Reading rumble values (uint8):
Rumble is only supported with OG Xbox and XInput so they will always be zero otherwise
```
#include "tusb_gamepad.h"

void read_rumble_values()
{
    Gamepad* gp = gamepad(0);

    uint8_t right_rumble = gp->rumble.r;
    uint8_t left_rumble  = gp->rumble.l;

    gp->reset_rumble(gp); // resets rumble values to zero
}
```
There are several function pointers within the gamepad object:
```
Gamepad* gp = gamepad(0);

gp->reset_pad(gp); // resets all buttons, triggers, joysticks
gp->reset_buttons(gp); // reset all analog and digital buttons
gp->reset_triggers(gp); // reset all triggers
gp->reset_joysticks(gp); // reset all joysticks
gp->reset_rumble(gp); // reset rumble values
```