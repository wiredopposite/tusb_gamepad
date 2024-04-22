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
You will need to add this line to your tusb_config.h file:
```
#include "board_config.h"
```
Optionally, you can create a tusb_gamepad_config.h file in your project to change some settings.
Here's how the file will look:

```
#ifndef _TUSB_GAMEPAD_CONFIG_H_
#define _TUSB_GAMEPAD_CONFIG_H_

// TUSB_GAMEPAD Options
#define MAX_GAMEPADS       1 // Max number of gamepads, this is used to add HID interfaces for PlayStation 3 and Switch, otherwise leave at 1.
#define TUSB_CDC_DEBUG     0 // Set to 1 for CDC device, helpful for print debugging USB host. Include utilities/log.h and use log() as you would printf().

/*  Only used for the RP2040 and currently only works with UART0 pins, 
    set InputMode to INPUT_MODE_UART_BRIDGE to use it as a UART bridge. 
    Include drivers/uartbridge/uart_bridge_task.h and use uart_bridge_task().
    The function doesn't return.*/
#define UART0_TX_PIN 16
#define UART0_RX_PIN 17

#endif // _TUSB_GAMEPAD_CONFIG_H_
```
The library will automatically redefine some things in your tusb_config.h file to make sure everything works properly with tusb_gamepad.

## Usage
This library is still in very eary development so usage is subject to change. 

### Template/example projects

I've made a couple of template projects to demonstrate integrating and using this library in your project. Integrating TinyUSB with ESP-IDF was troublesome for me at first so hopefully this helps.

- ESP-IDF: https://github.com/wiredopposite/tusb_gamepad_example_esp-idf
- PICO-SDK: 

### Interacting with the gamepad object
To change the gamepad object's button, trigger, and joystick values or read rumble values:

If you're only using one gamepad (MAX_GAMEPADS = 1), pass 0 as idx to gamepad(int idx):
```
#include "tusb_gamepad.h"

void update_gamepad()
{
    Gamepad* gp = gamepad(0);

    gp->reset_pad(gp);        // resets buttons, triggers, and joysticks
    gp->buttons.a    = true;  // buttons are bool
    gp->triggers.l   = 0xFF;  // triggers are uint8
    gp->joysticks.ly = 30000; // joysticks are int16
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