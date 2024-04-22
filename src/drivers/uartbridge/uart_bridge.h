#ifndef ESP_PLATFORM

#ifndef _UART_BRIDGE_H_
#define _UART_BRIDGE_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int itf;

void usbd_serial_init(int pico_uart0_tx_pin, int pico_uart0_rx_pin);
void update_uart_cfg(uint8_t itf);
void uart_write_bytes(uint8_t itf);
void init_uart_data(uint8_t itf);
void core1_entry(void);

#ifdef __cplusplus
}
#endif

#endif // _UART_BRIDGE_H_

#endif // ESP_PLATFORM