#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include "tusb_config.h"

#ifdef _TUSB_GAMEPAD_CONFIG_H_
    #include "tusb_gamepad_config.h"
#endif

#ifdef PICO_SDK_VERSION_STRING
    #ifndef MAX_GAMEPADS
        #define MAX_GAMEPADS 1
    #endif

    #ifndef UART0_TX_PIN
        #define UART0_TX_PIN 16
    #endif

    #ifndef UART0_RX_PIN
        #define UART0_RX_PIN 17
    #endif

#elif defined(ESP_PLATFORM)
    #include "sdkconfig.h"

    #ifdef CONFIG_BLUEPAD32_MAX_DEVICES
        #undef MAX_GAMEPADS
        #define MAX_GAMEPADS CONFIG_BLUEPAD32_MAX_DEVICES
    #else
        #ifndef MAX_GAMEPADS
            #define MAX_GAMEPADS 1
        #endif
    #endif

#endif 

// define min for ESP32
#ifndef MIN
    #define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

// TinyUSB config
// HID
#ifdef CFG_TUD_HID
    #if CFG_TUD_HID != (MAX_GAMEPADS + 1)
        #undef CFG_TUD_HID
        #define CFG_TUD_HID (MAX_GAMEPADS + 1)
    #endif
#else
    #define CFG_TUD_HID (MAX_GAMEPADS + 1)
#endif

#ifdef CFG_TUD_HID_EP_BUFSIZE
    #if (CFG_TUD_HID_EP_BUFSIZE < 64)
        #undef CFG_TUD_HID_EP_BUFSIZE
        #define CFG_TUD_HID_EP_BUFSIZE 64
    #endif
#else
    #define CFG_TUD_HID_EP_BUFSIZE 64
#endif

// CDC
#ifdef CFG_TUD_CDC
    #if (CFG_TUD_CDC < 1)
        #undef CFG_TUD_CDC
        #define CFG_TUD_CDC 1
    #endif
#else
    #define CFG_TUD_CDC 1
#endif

#ifndef CFG_TUD_CDC_RX_BUFSIZE
    #define CFG_TUD_CDC_RX_BUFSIZE 256
#elif (CFG_TUD_CDC_RX_BUFSIZE < 256)
    #undef CFG_TUD_CDC_RX_BUFSIZE
    #define CFG_TUD_CDC_RX_BUFSIZE 256
#endif

#ifndef CFG_TUD_CDC_TX_BUFSIZE
    #define CFG_TUD_CDC_TX_BUFSIZE 256
#elif (CFG_TUD_CDC_TX_BUFSIZE < 256)
    #undef CFG_TUD_CDC_TX_BUFSIZE
    #define CFG_TUD_CDC_TX_BUFSIZE 256
#endif

// RHPORT
#ifdef PICO_SDK_VERSION_STRING
    #define TUSB_GAMEPAD_RHPORT TUD_OPT_RHPORT
#elif defined(ESP_PLATFORM)
    #define TUSB_GAMEPAD_RHPORT BOARD_TUD_RHPORT
#else
    #error "Unsupported tusb_gamepad platform, Pico-SDK or ESP-IDF required"
#endif

#endif // _BOARD_CONFIG_H_