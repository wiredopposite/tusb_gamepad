#include <pico/stdlib.h>
#include "hardware/gpio.h"
#include "pico/multicore.h"

// #include "board_config.h"
#include "drivermanager.h"
#include "drivers/gpdriver.h"
#include "inputmodes.h"
#include "drivers/uartbridge/uart_bridge.h"
#include "drivers/uartbridge/uart_bridge_task.h"

// void uart_bridge_task()
// {
//     set_sys_clock_khz(250000, false);

//     DriverManager& driverManager = DriverManager::getInstance();
//     driverManager.setup(INPUT_MODE_UART_BRIDGE);

//     usbd_serial_init();

//     for (itf = 0; itf < CFG_TUD_CDC; itf++)
//         init_uart_data(itf);

//     // esp32_reset();

//     multicore_launch_core1(core1_entry);

//     while (1) 
//     {
//         uint8_t outBuffer[64];
//         GPDriver* driver = driverManager.getDriver();
//         driver->process(0, nullptr, outBuffer);
//     }
// }

// GPDriver* driver;

void init_uart_bridge(int uart0_tx_pin, int uart0_rx_pin)
{
    set_sys_clock_khz(250000, false);

    DriverManager& driverManager = DriverManager::getInstance();
    driverManager.setup(INPUT_MODE_UART_BRIDGE);

    usbd_serial_init(uart0_tx_pin, uart0_rx_pin);

    for (itf = 0; itf < CFG_TUD_CDC; itf++)
        init_uart_data(itf);
    
    multicore_launch_core1(core1_entry);
}

void uart_bridge_task()
{
    DriverManager& driverManager = DriverManager::getInstance();
    GPDriver* driver = driverManager.getDriver();

    uint8_t outBuffer[64];
    driver->process(0, nullptr, outBuffer);
}