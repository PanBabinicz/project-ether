#ifndef INC_I2C_CONTROLLER_H
#define INC_I2C_CONTROLLER_H

#include <stdint.h>
#include <stddef.h>
#include "driver/i2c.h"
#include "esp_log.h"
#include "hal/gpio_types.h"
#include "hal/i2c_types.h"
#include "soc/gpio_num.h"
#include "freertos/projdefs.h"

#define I2C_CONTROLLER_I2C_ACK_ENABLE   (0x01)
#define I2C_CONTROLLER_I2C_ACK_DISABLE  (0x00)

#define I2C_CONTROLLER_I2C_ACK   (0x00)
#define I2C_CONTROLLER_I2C_NACK  (0x01)

#define I2C_CONTROLLER_MASTER_SCL_IO          (GPIO_NUM_22)     /*!< gpio number for I2C master clock */
#define I2C_CONTROLLER_MASTER_SDA_IO          (GPIO_NUM_21)     /*!< gpio number for I2C master data  */
#define I2C_CONTROLLER_MASTER_FREQ_HZ         (100000)          /*!< I2C master clock frequency */
#define I2C_CONTROLLER_MASTER_TX_BUF_DISABLE  (0)               /*!< I2C master doesn't need buffer */
#define I2C_CONTROLLER_MASTER_RX_BUF_DISABLE  (0)               /*!< I2C master doesn't need buffer */

typedef enum {
  I2C_CONTROLLER_RESULT_SUCCESS = 0,
  I2C_CONTROLLER_RESULT_ERROR,
} i2c_controller_result_t;

typedef struct {
  i2c_config_t config;
  i2c_port_t i2c_num;
} i2c_controller_descriptor_t;

#define I2C_CONTROLLER_CONFIG_DEFAULT { \
  .mode = I2C_MODE_MASTER, \
  .sda_io_num = I2C_CONTROLLER_MASTER_SDA_IO, \
  .sda_pullup_en = GPIO_PULLUP_ENABLE, \
  .scl_io_num = I2C_CONTROLLER_MASTER_SCL_IO, \
  .scl_pullup_en = GPIO_PULLUP_ENABLE, \
  .master.clk_speed = I2C_CONTROLLER_MASTER_FREQ_HZ \
}

#define I2C_CONTROLLER_DESCRIPTOR_DEFAULT { \
  .config = I2C_CONTROLLER_CONFIG_DEFAULT, \
  .i2c_num = I2C_NUM_0 \
}

extern const i2c_controller_descriptor_t i2c_controller_descriptor_default;

i2c_controller_result_t i2c_controller_init(const i2c_controller_descriptor_t *descriptor);
i2c_controller_result_t i2c_controller_send(i2c_port_t i2c_num, uint8_t address, uint8_t reg, const uint8_t *data, size_t data_len);
i2c_controller_result_t i2c_controller_receive(i2c_port_t i2c_num, uint8_t address, uint8_t reg, uint8_t *data, size_t data_len);

#endif // !INC_I2C_CONTROLLER_H
