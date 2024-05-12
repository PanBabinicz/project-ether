#include "bme280.h"
#include "i2c_controller.h"
#include <stdint.h>

const bme280_settings_t bme280_default_settings = BME280_DEFAULT_SETTINGS;
static int32_t t_fine;

bme280_result_t bme280_init(i2c_port_t i2c_num, const bme280_settings_t *settings) 
{
  if (!settings) {
    return BME280_RESULT_ERROR;
  }

  i2c_controller_result_t result;

  /* CTRL_HUM Register settings. */
  result = i2c_controller_send(i2c_num, BME280_I2C_ADDRESS, 
                               BME280_REGISTER_CTRL_HUM, &settings->ctrl_hum, 
                               sizeof(settings->ctrl_hum));

  if (result != I2C_CONTROLLER_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  /* CTRL_MEAS Register settings. */
  result = i2c_controller_send(i2c_num, BME280_I2C_ADDRESS, 
                               BME280_REGISTER_CTRL_MEAS, &settings->ctrl_meas, 
                               sizeof(settings->ctrl_meas));

  if (result != I2C_CONTROLLER_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  /* CONFIG Register settings. */
  result = i2c_controller_send(i2c_num, BME280_I2C_ADDRESS, 
                               BME280_REGISTER_CONFIG, &settings->config, 
                               sizeof(settings->config));

  if (result != I2C_CONTROLLER_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  return BME280_RESULT_SUCCESS;
}

bme280_result_t bme280_reset(i2c_port_t i2c_num) 
{
  i2c_controller_result_t result;
  uint8_t data = BME280_DATA_RESET;

  result = i2c_controller_send(i2c_num, BME280_I2C_ADDRESS, 
                               BME280_REGISTER_RESET, &data, 
                               sizeof(data));

  if (result != I2C_CONTROLLER_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  return BME280_RESULT_SUCCESS;
}

bme280_result_t bme280_id(i2c_port_t i2c_num, uint8_t *data, size_t data_len) 
{
  if ((!data) || (data_len == 0)) {
    return BME280_RESULT_ERROR; 
  }
  
  i2c_controller_result_t result;

  result = i2c_controller_receive(i2c_num, BME280_I2C_ADDRESS, 
                                  BME280_REGISTER_ID, data, 
                                  data_len);

  if (result != I2C_CONTROLLER_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  return BME280_RESULT_SUCCESS;
}

bme280_result_t bme280_force_mode(i2c_port_t i2c_num, const bme280_settings_t *settings) 
{
  if (!settings) {
    return BME280_RESULT_ERROR;
  }

  i2c_controller_result_t result;

  result = i2c_controller_send(i2c_num, BME280_I2C_ADDRESS, 
                               BME280_REGISTER_CTRL_MEAS, &settings->ctrl_meas, 
                               sizeof(settings->ctrl_meas));

  if (result != I2C_CONTROLLER_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  return BME280_RESULT_SUCCESS;
}

bme280_result_t bme280_measure_humidity(i2c_port_t i2c_num, bme280_humidity_t *humidity) 
{
  if (!humidity) {
    return BME280_RESULT_ERROR;
  }

  i2c_controller_result_t result;
  uint8_t data[BME280_SIZE_HUM];

  result = i2c_controller_receive(i2c_num, BME280_I2C_ADDRESS, 
                                  BME280_REGISTER_HUM_MSB, data, 
                                  sizeof(data));

  if (result != I2C_CONTROLLER_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  humidity->msb = data[0];
  humidity->lsb = data[1];

  return BME280_RESULT_SUCCESS;
}

bme280_result_t bme280_measure_temperature(i2c_port_t i2c_num, 
                                           bme280_temperature_t *temperature) 
{
  if (!temperature) {
    return BME280_RESULT_ERROR;
  }

  i2c_controller_result_t result;
  uint8_t data[BME280_SIZE_TEMP];

  result = i2c_controller_receive(i2c_num, BME280_I2C_ADDRESS, 
                                  BME280_REGISTER_TEMP_MSB, data, 
                                  sizeof(data));

  if (result != I2C_CONTROLLER_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  temperature->msb   = data[0];
  temperature->lsb   = data[1];
  temperature->xlsb  = data[2] & 0xf0;

  return BME280_RESULT_SUCCESS;
}

bme280_result_t bme280_measure_pressure(i2c_port_t i2c_num, bme280_pressure_t *pressure) 
{
  if (!pressure) {
    return BME280_RESULT_ERROR;
  }

  i2c_controller_result_t result;
  uint8_t data[BME280_SIZE_PRESS];

  result = i2c_controller_receive(i2c_num, BME280_I2C_ADDRESS, 
                                  BME280_REGISTER_PRESS_MSB, data, 
                                  sizeof(data));

  if (result != I2C_CONTROLLER_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  pressure->msb   = data[0];
  pressure->lsb   = data[1];
  pressure->xlsb  = data[2] & 0xf0;

  return BME280_RESULT_SUCCESS;
}

bme280_result_t bme280_get_compensation_data(i2c_port_t i2c_num, 
                                             bme280_compensator_t *compensator) 
{
  if (!compensator) {
    return BME280_RESULT_ERROR;
  }

  i2c_controller_result_t result;
  uint8_t data[BME280_SIZE_COMP];
  uint8_t first_part = 24;
  uint8_t second_part = 1;
  uint8_t third_part = 7;

  result = i2c_controller_receive(i2c_num, BME280_I2C_ADDRESS, 
                                  BME280_REGISTER_CALIB00, data, 
                                  first_part);

  if (result != I2C_CONTROLLER_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  result = i2c_controller_receive(i2c_num, BME280_I2C_ADDRESS, 
                                  BME280_REGISTER_CALIB25, (data + first_part), 
                                  second_part);
  
  if (result != I2C_CONTROLLER_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  result = i2c_controller_receive(i2c_num, BME280_I2C_ADDRESS, 
                                  BME280_REGISTER_CALIB26, (data + first_part + second_part), 
                                  third_part);

  if (result != I2C_CONTROLLER_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  compensator->dig_t1 = ((data[1]   << 8) | data[0]);
  compensator->dig_t2 = ((data[3]   << 8) | data[2]);
  compensator->dig_t3 = ((data[5]   << 8) | data[4]);
  compensator->dig_p1 = ((data[7]   << 8) | data[6]);
  compensator->dig_p2 = ((data[9]   << 8) | data[8]);
  compensator->dig_p3 = ((data[11]  << 8) | data[10]);
  compensator->dig_p4 = ((data[13]  << 8) | data[12]);
  compensator->dig_p5 = ((data[15]  << 8) | data[14]);
  compensator->dig_p6 = ((data[17]  << 8) | data[16]);
  compensator->dig_p7 = ((data[19]  << 8) | data[18]);
  compensator->dig_p8 = ((data[21]  << 8) | data[20]);
  compensator->dig_p9 = ((data[23]  << 8) | data[22]);

  compensator->dig_h1 = (data[24]   << 8);
  compensator->dig_h2 = ((data[26]  << 8) | data[25]);
  compensator->dig_h3 = (data[27]   << 8);
  compensator->dig_h4 = (((data[28] << 4) | (data[29] & 0x0f)) & 0x0fff);
  compensator->dig_h5 = (((data[30] << 4) | (data[29] & 0xf0)) & 0x0fff);
  compensator->dig_h6 = data[31];

  return BME280_RESULT_SUCCESS;
}

bme280_result_t bme280_compensate_humidity(bme280_compensator_t *compensator, 
                                           bme280_humidity_t *humidity) 
{
  if ((!compensator) || (!humidity)) {
    return BME280_RESULT_ERROR;
  }

  int32_t adc_h = (((humidity->msb) << 8 ) | humidity->lsb);
  double var_h;

  var_h = (((double)t_fine) - 76800.0);
  var_h = (adc_h - (((double)compensator->dig_h4) * 64.0 + ((double)compensator->dig_h5) / 16384.0 * var_h)) * 
          (((double)compensator->dig_h2) / 65536.0 * (1.0 + ((double)compensator->dig_h6) / 67108864.0 * var_h * 
          (1.0 + ((double)compensator->dig_h3) / 67108864.0 * var_h)));
  var_h = var_h * (1.0 - ((double)compensator->dig_h1) * var_h / 524288.0);

  if (var_h > 100.0) {
    var_h = 100.0;
  } 
  else if (var_h < 0.0) {
    var_h = 0.0;
  }

  humidity->compensated = var_h;

  return BME280_RESULT_SUCCESS;
}

bme280_result_t bme280_compensate_temperature(bme280_compensator_t *compensator, 
                                              bme280_temperature_t *temperature) {
  if ((!compensator) || (!temperature)) {
    return BME280_RESULT_ERROR;
  }

  int32_t adc_t = (((temperature->msb) << 12) | ((temperature->lsb) << 4) | ((temperature->xlsb) >> 4));
  double var_1, var_2;

  var_1 = (((double)adc_t) / 16384.0 - ((double)compensator->dig_t1) / 1024.0) * ((double)compensator->dig_t2);
  var_2 = ((((double)adc_t) / 131072.0 - ((double)compensator->dig_t1) / 8192.0) * 
          (((double)adc_t) / 131072.0 - ((double)compensator->dig_t1) / 8192.0)) * ((double)compensator->dig_t3);

  t_fine = (int32_t)(var_1 + var_2);
  temperature->compensated = (var_1 + var_2) / 5120.0;

  return BME280_RESULT_SUCCESS;
}

bme280_result_t bme280_compensate_pressure(bme280_compensator_t *compensator, 
                                           bme280_pressure_t *pressure) {
  if ((!compensator) || (!pressure)) {
    return BME280_RESULT_ERROR;
  }

  int32_t adc_p = (((pressure->msb) << 12) | ((pressure->lsb) << 4) | ((pressure->xlsb) >> 4));
  double var_1, var_2;

  var_1 = ((double)t_fine / 2.0) - 64000.0;
  var_2 = var_1 * var_1 * ((double)compensator->dig_p6) / 32768.0;
  var_2 = var_2 + var_1 * ((double)compensator->dig_p5) * 2.0;
  var_2 = (var_2 / 4.0) + (((double)compensator->dig_p4) * 65536.0);
  var_1 = (((double)compensator->dig_p3) * var_1 * var_1 / 524288.0 + ((double)compensator->dig_p2) * var_1) / 524288.0;
  var_1 = (1.0 + var_1 / 32768.0) * ((double)compensator->dig_p1);

  if (var_1 == 0) {
    return BME280_RESULT_ERROR;
  }

  pressure->compensated = 1048576.0 - (double)adc_p;
  pressure->compensated = (pressure->compensated - (var_2 / 4096.0)) * 6250.0 / var_1;

  var_1 = ((double)compensator->dig_p9) * pressure->compensated * pressure->compensated / 2147483648.0;
  var_2 = pressure->compensated * ((double)compensator->dig_p8) / 32768.0;

  pressure->compensated = pressure->compensated + (var_1 + var_2 + ((double)compensator->dig_p7)) / 16.0;

  return BME280_RESULT_SUCCESS;
}
