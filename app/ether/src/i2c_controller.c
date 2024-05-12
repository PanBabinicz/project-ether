#include "i2c_controller.h"

const i2c_controller_descriptor_t i2c_controller_descriptor_default = I2C_CONTROLLER_DESCRIPTOR_DEFAULT;
static const TickType_t ticks = pdMS_TO_TICKS(10000);

i2c_controller_result_t i2c_controller_init(const i2c_controller_descriptor_t *descriptor) 
{
  if (!descriptor) {
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  esp_err_t result;
  static const char *I2C_CONTROLLER_CONFIG_TAG = "I2C_CONTROLLER_CONFIG";

  result = i2c_param_config(descriptor->i2c_num, &descriptor->config);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_CONFIG_TAG, "i2c_param_config result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  result = i2c_driver_install(descriptor->i2c_num, descriptor->config.mode, 
                              I2C_CONTROLLER_MASTER_RX_BUF_DISABLE, 
                              I2C_CONTROLLER_MASTER_TX_BUF_DISABLE, 0);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_CONFIG_TAG, "i2c_driver_install result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  ESP_LOGI(I2C_CONTROLLER_CONFIG_TAG, "i2c_controller_init: OK"); 
  return I2C_CONTROLLER_RESULT_SUCCESS;
}

i2c_controller_result_t i2c_controller_send(i2c_port_t i2c_num, uint8_t address, 
                                            uint8_t reg, const uint8_t *data, 
                                            size_t data_len) 
{
  esp_err_t result;
  static const char *I2C_CONTROLLER_SEND_TAG = "I2C_CONTROLLER_SEND";

  i2c_cmd_handle_t cmd = i2c_cmd_link_create();

  result = i2c_master_start(cmd);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_SEND_TAG, "i2c_master_start result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  /* Send device address. */
  result = i2c_master_write_byte(cmd, ((address << 1) | I2C_MASTER_WRITE), I2C_CONTROLLER_I2C_ACK_ENABLE);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_SEND_TAG, "i2c_master_write_byte result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  /* Send register address. */
  result = i2c_master_write_byte(cmd, reg, I2C_CONTROLLER_I2C_ACK_ENABLE);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_SEND_TAG, "i2c_master_write_byte result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  result = i2c_master_write(cmd, data, data_len, I2C_CONTROLLER_I2C_ACK_ENABLE);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_SEND_TAG, "i2c_master_write_byte result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  result = i2c_master_stop(cmd);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_SEND_TAG, "i2c_master_stop result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  result = i2c_master_cmd_begin(i2c_num, cmd, ticks);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_SEND_TAG, "i2c_master_cmd_begin result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  i2c_cmd_link_delete(cmd);

  ESP_LOGI(I2C_CONTROLLER_SEND_TAG, "i2c_controller_send: OK"); 

  return I2C_CONTROLLER_RESULT_SUCCESS;
}

i2c_controller_result_t i2c_controller_receive(i2c_port_t i2c_num, uint8_t address, 
                                               uint8_t reg, uint8_t *data, 
                                               size_t data_len) 
{
  esp_err_t result;
  static const char *I2C_CONTROLLER_RECEIVE_TAG = "I2C_CONTROLLER_RECEIVE";

  i2c_cmd_handle_t cmd = i2c_cmd_link_create();

  result = i2c_master_start(cmd);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_RECEIVE_TAG, "i2c_master_start result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  /* Send device address. */
  result = i2c_master_write_byte(cmd, ((address << 1) | I2C_MASTER_WRITE), I2C_CONTROLLER_I2C_ACK_ENABLE);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_RECEIVE_TAG, "i2c_master_write_byte result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  /* Send register address. */
  result = i2c_master_write(cmd, &reg, 1, I2C_CONTROLLER_I2C_ACK_ENABLE);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_RECEIVE_TAG, "i2c_master_write_byte result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  result = i2c_master_stop(cmd);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_RECEIVE_TAG, "i2c_master_stop result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  result = i2c_master_cmd_begin(i2c_num, cmd, ticks);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_RECEIVE_TAG, "i2c_master_cmd_begin result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  i2c_cmd_link_delete(cmd);
  cmd = i2c_cmd_link_create();

  result = i2c_master_start(cmd);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_RECEIVE_TAG, "i2c_master_start result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  /* Send device address. */
  result = i2c_master_write_byte(cmd, ((address << 1) | I2C_MASTER_READ), I2C_CONTROLLER_I2C_ACK_ENABLE);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_RECEIVE_TAG, "i2c_master_write_byte result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  if (data_len > 1) {
    result = i2c_master_read(cmd, data, data_len - 1, I2C_CONTROLLER_I2C_ACK);
    if (result != ESP_OK) {
      ESP_LOGI(I2C_CONTROLLER_RECEIVE_TAG, "i2c_master_read result = 0x%x", result); 
      return I2C_CONTROLLER_RESULT_ERROR;
    }
  }

  result = i2c_master_read_byte(cmd, data + (data_len - 1), I2C_CONTROLLER_I2C_NACK);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_RECEIVE_TAG, "i2c_master_read_byte result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  result = i2c_master_stop(cmd);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_RECEIVE_TAG, "i2c_master_stop result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  result = i2c_master_cmd_begin(i2c_num, cmd, ticks);
  if (result != ESP_OK) {
    ESP_LOGI(I2C_CONTROLLER_RECEIVE_TAG, "i2c_master_cmd_begin result = 0x%x", result); 
    return I2C_CONTROLLER_RESULT_ERROR;
  }

  i2c_cmd_link_delete(cmd);

  ESP_LOGI(I2C_CONTROLLER_RECEIVE_TAG, "i2c_controller_receive: OK"); 

  return I2C_CONTROLLER_RESULT_SUCCESS;
}
