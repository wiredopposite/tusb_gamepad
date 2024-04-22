#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h" 
#endif

#include "drivermanager.h"
#include "board_config.h"
#include "drivers/gpdriver.h"
#include "tusb_gamepad.h"

Gamepad local_gamepad;

void init_tusb_gamepad(enum InputMode mode)
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
            local_gamepad = *gamepad(i);
            uint8_t outBuffer[64];
            driver->process(i, &local_gamepad, outBuffer); // Dereference the pointer when passing to process
            driver->update_rumble(i, gamepad(i));       // Pass the pointer directly to update_rumble
        }

        #ifdef ESP_PLATFORM
        vTaskDelay(4);
        #endif
    }

    #ifdef ESP_PLATFORM
    vTaskDelay(4);
    #endif
}