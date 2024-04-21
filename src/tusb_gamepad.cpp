#if TUSB_GAMEPAD_MCU == MCU_ESP32S3
#include "freertos/FreeRTOS.h"
#include "freertos/task.h" 
#endif

#include "drivermanager.h"
#include "board_config.h"
#include "drivers/gpdriver.h"
#include "tusb_gamepad.h"

// DriverManager& driverManager;

void init_tusb_gamepad(InputMode mode)
{
    DriverManager& driverManager = DriverManager::getInstance();
    // driverManager = DriverManager::getInstance();
    driverManager.setup(mode);
}

void tusb_gamepad_task()
{
    DriverManager& driverManager = DriverManager::getInstance();
    GPDriver* driver = driverManager.getDriver();
    
    for (int i = 0; i < MAX_GAMEPADS; i++)
    {
        if (gamepad(i)) 
        {
            uint8_t outBuffer[64];
            driver->process(i, gamepad(i), outBuffer); // Dereference the pointer when passing to process
            driver->update_rumble(i, gamepad(i));       // Pass the pointer directly to update_rumble
        }

        #if TUSB_GAMEPAD_MCU == MCU_ESP32S3
        vTaskDelay(1);
        #endif
    }

    #if TUSB_GAMEPAD_MCU == MCU_ESP32S3
    vTaskDelay(1);
    #endif
}