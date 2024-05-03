#ifndef INC_UART_CONTROLLER_H
#define INC_UART_CONTROLLER_H

#include <stdint.h>
#include <stddef.h>
#include "driver/uart.h"
#include "hal/gpio_types.h"
#include "hal/uart_types.h"

typedef enum {
  UART_CONTROLLER_SUCCESS = 0,
  UART_CONTROLLER_ERROR,
} uart_controller_result_t;

typedef struct {
  const uart_config_t uart_config;
  const uart_port_t uart_port;
} uart_controller_descriptor_t;

#define UART_CONTROLLER_RX_BUF_SIZE (1024)
#define UART_CONTROLLER_TX_PIN      (GPIO_NUM_17)
#define UART_CONTROLLER_RX_PIN      (GPIO_NUM_16)

#define UART_CONTROLLER_CONFIG_DEFAULT  { \
  .baud_rate = 9600, \
  .data_bits = UART_DATA_8_BITS, \
  .parity = UART_PARITY_DISABLE, \
  .stop_bits = UART_STOP_BITS_1, \
  .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, \
  .source_clk = UART_SCLK_DEFAULT, \
}

#define UART_CONTROLLER_DESCRIPTOR_DEFAULT  { \
  .uart_config = UART_CONTROLLER_CONFIG_DEFAULT, \
  .uart_port = UART_NUM_2 \
}

extern const uart_controller_descriptor_t uart_controller_descriptor_default;

uart_controller_result_t uart_controller_init(const uart_controller_descriptor_t *uart_controller_descriptor);

#endif // !INC_UART_CONTROLLER_H