#include "board_config.h"
#include "drivermanager.h"

#include "drivers/hid/HIDDriver.h"
#include "drivers/dinput/DInputDriver.h"
#include "drivers/ps3/PS3Driver.h"
#include "drivers/psclassic/PSClassicDriver.h"
#include "drivers/switch/SwitchDriver.h"
#include "drivers/xboxog/XboxOriginalDriver.h"
#include "drivers/xinput/XInputDriver.h"
#include "drivers/usbserial/USBSerialDriver.h"
#include "drivers/uartbridge/UARTBridgeDriver.h"

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
        case INPUT_MODE_USBSERIAL:
            driver = new USBSerialDriver();
            break;
        #ifdef _UARTBRIDGEDRIVER_H_
        case INPUT_MODE_UART_BRIDGE:
            driver = new UARTBridgeDriver();
            break;
        #endif
        default:
            return;
    }
    
    driver->initialize();

    tud_init(TUSB_GAMEPAD_RHPORT);
}
