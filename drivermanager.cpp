#include "usbd/drivermanager.h"

// #include "usbd/drivers/hid/HIDDriver.h"
#include "usbd/drivers/dinput/DInputDriver.h"
// #include "usbd/drivers/ps3/PS3Driver.h"
#include "usbd/drivers/psclassic/PSClassicDriver.h"
#include "usbd/drivers/switch/SwitchDriver.h"
#include "usbd/drivers/xboxog/XboxOriginalDriver.h"
#include "usbd/drivers/xinput/XInputDriver.h"
#include "usbd/drivers/usbserial/USBSerialDriver.h"
#include "usbd/drivers/uartbridge/UARTBridgeDriver.h"

void DriverManager::setup(InputMode mode) 
{
    switch (mode) 
    {
        case INPUT_MODE_HID:
            // driver = new HIDDriver();
            driver = new DInputDriver();
            // driver = new PS3Driver();
            break;
        case INPUT_MODE_PSCLASSIC:
            driver = new PSClassicDriver();
            break;
        case INPUT_MODE_SWITCH:
            driver = new SwitchDriver();
            break;
        case INPUT_MODE_XBOXORIGINAL:
            driver = new XboxOriginalDriver();
            break;
        case INPUT_MODE_XINPUT:
            driver = new XInputDriver();
            break;
        #ifdef _USB_SERIAL_DRIVER_H_
        case INPUT_MODE_USBSERIAL:
            driver = new USBSerialDriver();
            break;
        #endif
        #ifdef _UARTBRIDGEDRIVER_H_
        case INPUT_MODE_UART_BRIDGE:
            driver = new UARTBridgeDriver();
            break;
        #endif
        default:
            return;
    }
    
    // Initialize our chosen driver
    driver->initialize();

    #if (OGX_MCU != MCU_ESP32S3)
    tud_init(TUD_OPT_RHPORT);
    #else
    tud_init(BOARD_TUD_RHPORT);
    #endif
}
