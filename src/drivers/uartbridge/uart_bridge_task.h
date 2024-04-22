#ifndef ESP_PLATFORM

#ifndef _UART_BRIDGE_TASK_H_
#define _UART_BRIDGE_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

void init_uart_bridge(int uart0_tx_pin, int uart0_rx_pin);
void uart_bridge_task();

#ifdef __cplusplus
}
#endif

#endif // _UART_BRIDGE_TASK_H_

#endif // ESP_PLATFORM