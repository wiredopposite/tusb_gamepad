#ifndef BOARD_CONFIG_H_
#define BOARD_CONFIG_H_

#include "usbd_config.h"
#include "usbd/usbd_boards.h"

#ifndef USBD_BOARD
    #error USBD_BOARD must be defined in usbd_config.h
#endif

// Type
#define WIRED    1
#define WIRELESS 2
// MCU
#define MCU_RP2040  1
#define MCU_ESP32S3 2

#if USBD_BOARD != OGXW_LITE
    #define OGX_MCU MCU_RP2040
#else
    #define OGX_MCU MCU_ESP32S3
#endif

#if (USBD_BOARD == OGXM_ADA_FEATHER_USBH) || (USBD_BOARD == OGXM_PI_PICO) || (USBD_BOARD == OGXM_RPZERO_INTERPOSER)
    #define OGX_TYPE WIRED
#else
    #define OGX_TYPE WIRELESS
#endif

#if OGX_TYPE == WIRED
    #ifdef USBD_MAX_GAMEPADS
        #define MAX_GAMEPADS USBD_MAX_GAMEPADS
    #else
        #define MAX_GAMEPADS 1
    #endif

    #if USBD_BOARD == OGXM_ADA_FEATHER_USBH
        #define PIO_USB_DP_PIN    16 // DM = 17
        #define LED_INDICATOR_PIN 13
        #define VCC_EN_PIN        18
        // #define NEOPIXEL_PWR_PIN    20
        // #define NEOPIXEL_CTRL_PIN   21 

    #elif USBD_BOARD == OGXM_PI_PICO
        #define PIO_USB_DP_PIN    0 // DM = 1
        #define LED_INDICATOR_PIN 25

    #elif USBD_BOARD == OGXM_RPZERO_INTERPOSER
        #define PIO_USB_DP_PIN    10 // DM = 11
        #define LED_INDICATOR_PIN 13
        
    #endif

#elif OGX_TYPE == WIRELESS
    #if OGX_MCU == MCU_ESP32S3
        #include "sdkconfig.h"

        #ifdef CONFIG_BLUEPAD32_MAX_DEVICES
            #define MAX_GAMEPADS CONFIG_BLUEPAD32_MAX_DEVICES
        #endif
    #else
        #ifdef USBD_MAX_GAMEPADS
            #define MAX_GAMEPADS USBD_MAX_GAMEPADS
        #else
            #define MAX_GAMEPADS 1
        #endif
    #endif 

    #define PLAYER_ID1_PIN 2
    #define PLAYER_ID2_PIN 3

    #if (USBD_BOARD == OGXW_RETAIL_1CH)
        #define I2C1_SLAVE_SDA_PIN 18
        #define I2C1_SLAVE_SCL_PIN 19

        #define MODE_SEL_PIN 21

        #define ESP_PROG_PIN 20 // ESP32 IO0
        #define ESP_RST_PIN 8   // ESP32 EN

        #define UART0_TX_PIN 16
        #define UART0_RX_PIN 17

    #elif (USBD_BOARD == OGXW_RPZERO_1CH)
        #define I2C1_SLAVE_SDA_PIN 10
        #define I2C1_SLAVE_SCL_PIN 11

        #define MODE_SEL_PIN 9

        #define ESP_PROG_PIN 15 // ESP32 IO0
        #define ESP_RST_PIN 14  // ESP32 EN

        #define UART0_TX_PIN 12
        #define UART0_RX_PIN 13

    #elif (USBD_BOARD == OGXW_RPZERO_2CH)
        #define I2C1_SLAVE_SDA_PIN 10
        #define I2C1_SLAVE_SCL_PIN 11

        #define MODE_SEL_PIN 8

        #define ESP_PROG_PIN 9 // ESP32 IO0
        #define ESP_RST_PIN  7  // ESP32 EN

        #define UART0_TX_PIN 12
        #define UART0_RX_PIN 13

    #endif
#endif

#ifndef CDC_DEBUG
    #define CDC_DEVICES 0
#elif (CDC_DEBUG > 0)
    #define CDC_DEVICES 1
#else
    #define CDC_DEVICES 0
#endif

#ifndef MIN
    #define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#endif // BOARD_CONFIG_H_