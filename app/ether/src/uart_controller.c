#include "uart_controller.h"

const uart_controller_descriptor_t uart_controller_descriptor_default = UART_CONTROLLER_DESCRIPTOR_DEFAULT;

uart_controller_result_t uart_controller_init(const uart_controller_descriptor_t *uart_controller_descriptor) 
{
  if (!uart_controller_descriptor) {
    return UART_CONTROLLER_RESULT_ERROR;
  }

  uart_driver_install(uart_controller_descriptor->uart_port, 
                      UART_CONTROLLER_RX_BUF_SIZE * 2, 0, 0, NULL, 0);

  uart_param_config(uart_controller_descriptor->uart_port, 
                    &uart_controller_descriptor->uart_config);

  uart_set_pin(uart_controller_descriptor->uart_port, UART_CONTROLLER_TX_PIN, 
               UART_CONTROLLER_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

  return UART_CONTROLLER_RESULT_SUCCESS;
}
