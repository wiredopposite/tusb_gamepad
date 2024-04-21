# tusb_gamepad
A library that can emulate gamepads for several platforms using TinyUSB. Currently it's only usuable with the Raspberry Pi RP2040 (Pico) and Espressif chips with built in USB (though it's only been tested on the ESP32S3).

## Supported platforms
- Original Xbox
- XInput
- PlayStation 3
- PlayStation Classic
- Nintendo Switch

## Requirements
You will need TinyUSB integrated into your project, tusb_gamepad implements all necessary TinyUSB callbacks for you. I'll go over how to do that with the ESP-IDF specifically since it was a pain to figure out at first. You'll want to clone this repository into the components folder of your project, then ```git clone --recursive https://github.com/hathach/tinyusb.git``` into your project (somewhere else like a lib folder is probably best).
### ESP-IDF
Here's an example CMakeLists.txt file at the root of the project integrating TinyUSB:
```
cmake_minimum_required(VERSION 3.17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(FAMILY espressif)
set(BOARD espressif_s3_devkitc) // or whichever board you're using, the options are the folder names in tinyusb/hw/bsp/espressif/boards

set(TINYUSB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/lib/tinyusb) // or wherever you put tinyusb
include(${TINYUSB_ROOT}/hw/bsp/family_support.cmake)

family_get_project_name(PROJECT ${CMAKE_CURRENT_LIST_DIR})

project(${PROJECT} C CXX ASM)

family_initialize_project(${PROJECT} ${CMAKE_CURRENT_LIST_DIR})
```
Then in your "src" or "main" directory CMakeLists.txt file, you'll add boards, tinyusb_src, and tusb_gamepad to your list of requires
```
idf_component_register( SRCS "main.c" 
                        INCLUDE_DIRS "."
                        REQUIRES boards tinyusb_src tusb_gamepad)
```
### Config file
Similar to TinyUSB, this library needs a tusb_gamepad_config.h file in the project defining the MCU and the number of gamepads you're using, here's how that will look:

```
#ifndef TUSB_GAMEPAD_CONFIG_H_
#define TUSB_GAMEPAD_CONFIG_H_

// TUSB_GAMEPAD_MCUs
// MCU_RP2040 
// MCU_ESPRESSIF_USB

// TUSB_GAMEPAD Options
#define TUSB_GAMEPAD_MCU   MCU_ESPRESSIF_USB
#define MAX_GAMEPADS       1 // Max number of gamepads, this is used to add HID interfaces for PlayStation 3, otherwise leave at 1.
#define TUSB_CDC_DEBUG     0 // Only used for the RP2040. Set to 1 for CDC device, helpful for print debugging USB host. Include utilities/log.h and use log() as you would printf()

// Only used for the RP2040 and currently only works with UART0 pins, set InputMode to INPUT_MODE_UART_BRIDGE to use the RP2040 as a UART bridge. This setting is ignored otherwise.
// #define UART0_TX_PIN 16 
// #define UART0_RX_PIN 17

#endif // TUSB_GAMEPAD_CONFIG_H_
```
The library will automatically redefine some things in your tusb_config.h file based on your choices here to make sure everything works properly with your settings.

## Usage
This library is still in very eary development so usage is subject to change. 

You choose an InputMode from these options:
```
enum InputMode
{
    INPUT_MODE_XINPUT,
    INPUT_MODE_SWITCH,
    INPUT_MODE_HID,
    INPUT_MODE_PSCLASSIC,
    INPUT_MODE_XBOXORIGINAL,
    INPUT_MODE_USBSERIAL,
    INPUT_MODE_UART_BRIDGE,
};
```

INPUT_MODE_USBSERIAL and INPUT_MODE_UART_BRIDGE are both for the RP2040 only and there incase you'd like to use it as a UART programmer (INPUT_MODE_UART_BRIDGE) or get debug print statements via USB (INPUT_MODE_USBSERIAL). 

A more detailed explanation of these modes is at the bottom of this README.

### With the RP2040
Here's an example of how you'd structure your main function and loop to use this library on the Pico:

```
#include "bsp/board_api.h" // from tinyusb
#include "tusb_gamepad.h"

int main()
{
    board_init();

    InputMode input_mode = INPUT_MODE_XINPUT;

    init_tusb_gamepad(input_mode); // initializes the gamepad driver and tinyusb device stack

    while(1)
    {
        tusb_gamepad_task(); // sends/receives USB reports, updates the gamepad object
        tud_task();
    }

    return 0;
}
```

### With the ESP32S3
Freertos is a bit different with how TinyUSB is used, here's an example of a main.c file on the ESP32S3:



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
void update_gamepad()
{
    gamepad(0)->buttons.a    = true; 
    gamepad(0)->triggers.l   = 0xFF; 
    gamepad(0)->joysticks.ly = 30000;
}
```

If MAX_GAMEPADS is set to greater than 1, idx for gamepad(int idx) should be between 0 and MAX_GAMEPADS-1, otherwise a null is returned
```
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
void read_rumble_values()
{
    Gamepad* gp = gamepad(0);

    uint8_t right_rumble = gp->rumble.r;
    uint8_t left_rumble  = gp->rumble.l;

    gp->reset_rumble(gp); // resets rumble values to zero
}
```