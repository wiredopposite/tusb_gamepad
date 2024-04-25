#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h" 
#endif

#include "drivermanager.h"
#include "board_config.h"
#include "drivers/gpdriver.h"
#include "tusb_gamepad.h"

Gamepad gamepad_copy;
Gamepad* gp[MAX_GAMEPADS] = {NULL};
uint8_t outBuffer[64];

void init_tusb_gamepad(enum InputMode mode)
{
    DriverManager& driverManager = DriverManager::getInstance();
    driverManager.setup(mode);

    for (int i = 0; i < MAX_GAMEPADS; i++)
    {
       gp[i] = gamepad(i);
    }
}

void tusb_gamepad_task()
{
    DriverManager& driverManager = DriverManager::getInstance();
    GPDriver* driver = driverManager.getDriver();
    
    for (int i = 0; i < MAX_GAMEPADS; i++)
    {
        if (gp[i] != NULL) 
        {
            gamepad_copy = *gamepad(i);
            driver->process(i, &gamepad_copy, outBuffer);
            driver->update_rumble(i, gp[i]);
        }

        #ifdef ESP_PLATFORM
        vTaskDelay(4);
        #endif
    }

    #ifdef ESP_PLATFORM
    vTaskDelay(4);
    #endif
}