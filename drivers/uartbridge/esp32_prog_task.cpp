#ifdef ESP32_PROG_TASK_H_

#include <pico/stdlib.h>
#include "hardware/gpio.h"
#include "pico/multicore.h"

#include "usbd/board_config.h"
#include "usbd/drivermanager.h"
#include "usbd/drivers/gpdriver.h"
#include "usbd/inputmodes.h"
#include "usbd/drivers/uartbridge/uart_bridge.h"
#include "usbd/drivers/uartbridge/esp32_prog_task.h"

void esp32_reset() 
{
    gpio_init(ESP_PROG_PIN);
    gpio_set_dir(ESP_PROG_PIN, GPIO_OUT);
    gpio_put(ESP_PROG_PIN, 1);

    gpio_init(ESP_RST_PIN);
    gpio_set_dir(ESP_RST_PIN, GPIO_OUT);
    gpio_put(ESP_PROG_PIN, 1);

    gpio_put(ESP_PROG_PIN, 0);
	sleep_ms(250);

    gpio_put(ESP_RST_PIN, 0);
    sleep_ms(500);
    gpio_put(ESP_RST_PIN, 1);
	sleep_ms(250);
	gpio_put(ESP_PROG_PIN, 1);
}

void esp32_programming_task()
{
    set_sys_clock_khz(250000, false);

    DriverManager& driverManager = DriverManager::getInstance();
    driverManager.setup(INPUT_MODE_UART_BRIDGE);

    usbd_serial_init();

    for (itf = 0; itf < CFG_TUD_CDC; itf++)
        init_uart_data(itf);

    esp32_reset();

    multicore_launch_core1(core1_entry);

    while (1) 
    {
        uint8_t outBuffer[64];
        GPDriver* driver = driverManager.getDriver();
        driver->process(0, nullptr, outBuffer);
    }
}

#endif // ESP32_PROG_TASK_H_