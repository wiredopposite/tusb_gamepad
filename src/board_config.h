#ifndef BOARD_CONFIG_H_
#define BOARD_CONFIG_H_

#include "tusb_config.h"
#include "tusb_option.h"
#include "tusb_gamepad_config.h"

#ifndef TUSB_GAMEPAD_MCU
    #error TUD_GAMEPAD_MCU must be defined in tusb_gamepad_config.h
#endif

// MCU
#define MCU_RP2040  1
#define MCU_ESP32S3 2

#if (TUSB_GAMEPAD_MCU == MCU_RP2040)
    #ifndef MAX_GAMEPADS
        #define MAX_GAMEPADS 1
    #endif

    #ifndef UART0_TX_PIN
        #define UART0_TX_PIN 16
    #endif

    #ifndef UART0_RX_PIN
        #define UART0_RX_PIN 17
    #endif

#elif (TUSB_GAMEPAD_MCU == MCU_ESP32S3)
    #include "sdkconfig.h"

    #ifdef CONFIG_BLUEPAD32_MAX_DEVICES
        #undef MAX_GAMEPADS
        #define MAX_GAMEPADS CONFIG_BLUEPAD32_MAX_DEVICES
    #else
        #ifndef MAX_GAMEPADS
            #define MAX_GAMEPADS 1
        #endif
    #endif

#else
    #ifdef TUSB_MAX_GAMEPADS
        #define MAX_GAMEPADS TUSB_MAX_GAMEPADS
    #else
        #define MAX_GAMEPADS 1
    #endif

#endif 

#ifndef MIN
    #define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

// TinyUSB config
#ifdef CFG_TUD_HID_EP_BUFSIZE
    #if (CFG_TUD_HID_EP_BUFSIZE < 64)
        #undef CFG_TUD_HID_EP_BUFSIZE
        #define CFG_TUD_HID_EP_BUFSIZE 64
    #endif
#else
    #define CFG_TUD_HID_EP_BUFSIZE 64
#endif

#if TUSB_GAMEPAD_MCU == MCU_RP2040
    #ifndef TUSB_CDC_DEBUG
        #define CDC_DEVICES 0
    #elif (TUSB_CDC_DEBUG > 0)
        #define CDC_DEVICES 1
    #else
        #define CDC_DEVICES 0
    #endif

    #ifdef CFG_TUD_CDC
        #undef CFG_TUD_CDC
        #define CFG_TUD_CDC CDC_DEVICES
    #else
        #define CFG_TUD_CDC CDC_DEVICES
    #endif
#endif

#ifdef CFG_TUD_HID
    #undef CFG_TUD_HID
    #define CFG_TUD_HID (MAX_GAMEPADS + 1)
#else
    #define CFG_TUD_HID (MAX_GAMEPADS + 1)
#endif

#if TUSB_GAMEPAD_MCU == MCU_RP2040
    #define TUSB_GAMEPAD_RHPORT TUD_OPT_RHPORT
#elif TUSB_GAMEPAD_MCU == MCU_ESP32S3
    #define TUSB_GAMEPAD_RHPORT BOARD_TUD_RHPORT
#endif

#endif // BOARD_CONFIG_H_