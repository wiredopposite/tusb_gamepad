# tusb_gamepad
A library that can emulate gamepads for several platforms using TinyUSB. Currently it's compatible with the Raspberry Pi RP2040 (Pico) and Espressif chips with built in USB (though it's only been tested on the ESP32S3).

## Supported platforms
- Original Xbox
- XInput (works on Xbox 360 with [UsbdSecPatch](https://github.com/InvoxiPlayGames/UsbdSecPatch))
- PlayStation 3
- PlayStation Classic
- Nintendo Switch

## Usage/requirements
This library is still in very eary development so usage is subject to change. 

You will need TinyUSB added to your project, tusb_gamepad implements all necessary TinyUSB device callbacks for you. 

### tinyusb
You will need to add these lines to your tusb_config.h file:
```
#include "board_config.h"

#define MAX_GAMEPADS 1 // or up to 4 if using DInput/PlayStation 3
```
And also download the TinyUSB dependancies for your board if you're using the ESP-IDF:
```
cd tinyusb
python tools/get_deps.py your_board
```
The board options for Espressif are in tinyusb/hw/bsp/espressif/boards

While compiling, the library will automatically redefine some TinyUSB settings to make sure everything works properly with tusb_gamepad. To avoid getting redefine warnings from the compiler, you can just use a tusb_config.h file from one of these example projects listed below.

### Template/example projects
Here are a few template projects meant to demonstrate integrating and using this library in your project. Take note of the CMakeLists.txt files in each.

- ESP-IDF: 
    - [Basic device example](https://github.com/wiredopposite/tusb_gamepad_example_esp-idf)
- PICO-SDK: 
    - [Basic device example](https://github.com/wiredopposite/tusb_gamepad_example_pico-sdk)
    - [Dualshock 4 to XInput device/host (w pico-pio-usb)](https://github.com/wiredopposite/tusb_gamepad_ds4_to_xinput_pico-sdk)

### Interacting with the gamepad object
If you're only using one gamepad (MAX_GAMEPADS = 1), pass 0 as the parameter to the gamepad function:
```
#include "tusb_gamepad.h"

void update_gamepad()
{
    Gamepad* gp = gamepad(0); // get pointer to the object

    gp->reset_pad(gp);        // resets buttons, triggers, and joysticks

    // Ways to enable buttons, which are part of a 16bit button mask
    gp->buttons.a   = 1;
    gp->buttons.x   = true;

    gp->triggers.l   = 0xFF;  // triggers are uint8_t
    gp->joysticks.ly = 30000; // joysticks are full range int16_t
}
```
Or alternatively you can use the gamepad function directly, but this can add a little overhead if you're constantly doing it
```
#include "tusb_gamepad.h"

void update_gamepad()
{
    gamepad(0)->buttons.a    = true; 
    gamepad(0)->triggers.l   = 0xFF; 
    gamepad(0)->joysticks.ly = 30000;
}
```
There is also support for analog buttons for DInput (PlayStation 3) and Original Xbox. To enable this functionality, you only need to do this once:
```
gamepad(0)->enable_analog_buttons = true;
```
The analog buttons will be enabled from that point onward, or until you set enable_analog_buttons to false.
```
#include "tusb_gamepad.h"

Gamepad* gp = gamepad(0);

void update_analog_buttons()
{
    gp->analog_buttons.a = 0xFF;
    gp->analog_buttons.x = 0xFF / 2;
    gp->analog_buttons.y = 0xFF / 4;
}
```

If MAX_GAMEPADS is set to greater than 1, idx for gamepad(int idx) should be between 0 and MAX_GAMEPADS-1, otherwise a null is returned. Setting MAX_GAMEPADS to great than 1 will expose additional USB HID interfaces while using INPUT_MODE_DINPUT, each being treated as their own gamepad.
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