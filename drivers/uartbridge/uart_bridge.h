#ifndef _UART_BRIDGE_H_
#define _UART_BRIDGE_H_

#if (OGX_TYPE == WIRELESS) && (OGX_MCU == MCU_RP2040)

#ifdef __cplusplus
extern "C" {
#endif

extern int itf;

void usbd_serial_init(void);
void update_uart_cfg(uint8_t itf);
void uart_write_bytes(uint8_t itf);
void init_uart_data(uint8_t itf);
void core1_entry(void);

#ifdef __cplusplus
}
#endif

#endif // (OGX_TYPE == WIRELESS) && (OGX_MCU == MCU_RP2040)

#endif // _UART_BRIDGE_H_