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

#define I2C_CONTROLLER_MASTER_SCL_IO          (GPIO_NUM_22)     /*!< GPIO number for I2C master clock. */
#define I2C_CONTROLLER_MASTER_SDA_IO          (GPIO_NUM_21)     /*!< GPIO number for I2C master data.  */
#define I2C_CONTROLLER_MASTER_FREQ_HZ         (100000)          /*!< I2C master clock frequency. */
#define I2C_CONTROLLER_MASTER_TX_BUF_DISABLE  (0)               /*!< I2C master doesn't need buffer. */
#define I2C_CONTROLLER_MASTER_RX_BUF_DISABLE  (0)               /*!< I2C master doesn't need buffer. */

/** 
 * \brief Result codes for I2C controller operations.
 */
typedef enum {
  I2C_CONTROLLER_RESULT_SUCCESS = 0,  /*!< Operation was successful. */
  I2C_CONTROLLER_RESULT_ERROR,        /*!< Operation encountered an error. */
} i2c_controller_result_t;

/** 
 * \brief Structure for I2C controller descriptor.
 */
typedef struct {
  i2c_config_t config;          /*!< I2C configuration. */
  i2c_port_t i2c_num;           /*!< I2C port number. */
} i2c_controller_descriptor_t;

/** 
 * \brief Default configuration for the I2C controller.
 */
#define I2C_CONTROLLER_CONFIG_DEFAULT {               \
  .mode = I2C_MODE_MASTER,                            \
  .sda_io_num = I2C_CONTROLLER_MASTER_SDA_IO,         \
  .sda_pullup_en = GPIO_PULLUP_ENABLE,                \
  .scl_io_num = I2C_CONTROLLER_MASTER_SCL_IO,         \
  .scl_pullup_en = GPIO_PULLUP_ENABLE,                \
  .master.clk_speed = I2C_CONTROLLER_MASTER_FREQ_HZ,  \
}

/** 
 * \brief Default descriptor for the I2C controller.
 */
#define I2C_CONTROLLER_DESCRIPTOR_DEFAULT { \
  .config = I2C_CONTROLLER_CONFIG_DEFAULT,  \
  .i2c_num = I2C_NUM_0,                     \
}

/** 
 * \brief Initialize the I2C controller.
 * 
 * \param[in]   descriptor: Pointer to the I2C controller descriptor.
 * \return      Result of the initialization.
 */
i2c_controller_result_t i2c_controller_init(const i2c_controller_descriptor_t *descriptor);

/** 
 * \brief Send data over I2C.
 * 
 * \param[in]   i2c_num: I2C port number.
 * \param[in]   address: I2C address of the device.
 * \param[in]   reg: Register address to send data to.
 * \param[in]   data: Pointer to the data to send.
 * \param[in]   data_len: Length of the data to send.
 * \return      Result of the send operation.
 */
i2c_controller_result_t i2c_controller_send(i2c_port_t i2c_num, uint8_t address, 
                                            uint8_t reg, const uint8_t *data, 
                                            size_t data_len);

/** 
 * \brief Receive data over I2C.
 * 
 * \param[in]   i2c_num: I2C port number.
 * \param[in]   address: I2C address of the device.
 * \param[in]   reg: Register address to receive data from.
 * \param[out]  data: Pointer to the buffer to store received data.
 * \param[in]   data_len: Length of the data to receive.
 * \return      Result of the receive operation.
 */
i2c_controller_result_t i2c_controller_receive(i2c_port_t i2c_num, uint8_t address, 
                                               uint8_t reg, uint8_t *data, 
                                               size_t data_len);

#endif // !INC_I2C_CONTROLLER_H
