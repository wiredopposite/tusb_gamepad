#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

// #include "tusb_config.h"

#ifndef ESP_PLATFORM
    #include "pico/version.h"
#endif

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef PICO_SDK_VERSION_STRING
    #error "PICO_SDK_VERSION_STRING is not defined" // Assuming this matches the SDK version you use
#endif

#ifdef _TUSB_GAMEPAD_CONFIG_H_
    #include "tusb_gamepad_config.h"
#endif

#ifndef MAX_GAMEPADS
    #ifdef PICO_SDK_VERSION_STRING
        #define MAX_GAMEPADS 1
    #elif defined(ESP_PLATFORM)
        #include "sdkconfig.h"
        #ifdef CONFIG_BLUEPAD32_MAX_DEVICES
            #define MAX_GAMEPADS CONFIG_BLUEPAD32_MAX_DEVICES
        #else
            #define MAX_GAMEPADS 1
        #endif
    #endif
#endif

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
#ifdef TUD_OPT_RHPORT
    #define TUSB_GAMEPAD_RHPORT TUD_OPT_RHPORT
#elif defined(BOARD_TUD_RHPORT)
    #define TUSB_GAMEPAD_RHPORT BOARD_TUD_RHPORT
#else
    #define TUSB_GAMEPAD_RHPORT 0
#endif

#ifdef __cplusplus
    }
#endif

#endif // _BOARD_CONFIG_H_