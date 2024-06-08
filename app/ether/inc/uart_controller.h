#ifndef INC_UART_CONTROLLER_H
#define INC_UART_CONTROLLER_H

#include <stdint.h>
#include <stddef.h>
#include "driver/uart.h"
#include "hal/gpio_types.h"
#include "hal/uart_types.h"

#define UART_CONTROLLER_RX_BUF_SIZE (1024)
#define UART_CONTROLLER_TX_PIN      (GPIO_NUM_17)
#define UART_CONTROLLER_RX_PIN      (GPIO_NUM_16)

/** 
 * \brief Result codes for UART controller operations.
 */
typedef enum {
  UART_CONTROLLER_RESULT_SUCCESS = 0,   /*!< Operation was successful. */
  UART_CONTROLLER_RESULT_ERROR,         /*!< Operation encountered an error. */
} uart_controller_result_t;

/** 
 * \brief Structure for UART controller descriptor.
 */
typedef struct {
  uart_config_t uart_config;      /*!< UART configuration. */
  uart_port_t uart_port;          /*!< UART port number. */
} uart_controller_descriptor_t;

/** 
 * \brief Default configuration for the UART controller.
 */
#define UART_CONTROLLER_CONFIG_DEFAULT  { \
  .baud_rate = 9600,                      \
  .data_bits = UART_DATA_8_BITS,          \
  .parity = UART_PARITY_DISABLE,          \
  .stop_bits = UART_STOP_BITS_1,          \
  .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,  \
  .source_clk = UART_SCLK_DEFAULT,        \
}

/** 
 * \brief Default descriptor for the UART controller.
 */
#define UART_CONTROLLER_DESCRIPTOR_DEFAULT  {     \
  .uart_config = UART_CONTROLLER_CONFIG_DEFAULT,  \
  .uart_port = UART_NUM_2,                        \
}

/** 
 * \brief Initialize the UART controller.
 * 
 * \param[in]   uart_controller_descriptor: Pointer to the UART controller descriptor.
 * \return      Result of the initialization operation.
 */
uart_controller_result_t uart_controller_init(const uart_controller_descriptor_t *uart_controller_descriptor);

#endif // !INC_UART_CONTROLLER_H
