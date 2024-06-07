#include "state_machine.h"

static const char *STATE_MACHINE_TAG = "STATE_MACHINE";

bme280_result_t state_machine_bme280_init(ether_t *ether)
{
  if (!ether) {
    return BME280_RESULT_ERROR;
  }

  bme280_result_t result = bme280_init(ether->descriptor.i2c_controller.i2c_num, &ether->settings.bme280);

#if defined(ETHER_DEBUG)
  ESP_LOGI(STATE_MACHINE_TAG, "BME280_STATE_INIT");
  ESP_LOGI(STATE_MACHINE_TAG, "RESULT: %d", result);
#endif

  if (result != BME280_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  ether->state_machine.bme280 = BME280_STATE_ID;

  return BME280_RESULT_SUCCESS;
}


bme280_result_t state_machine_bme280_reset(ether_t *ether)
{
  if (!ether) {
    return BME280_RESULT_ERROR;
  }

  bme280_result_t result = bme280_reset(ether->descriptor.i2c_controller.i2c_num);

#if defined(ETHER_DEBUG)
  ESP_LOGI(STATE_MACHINE_TAG, "BME280_STATE_RESET");
  ESP_LOGI(STATE_MACHINE_TAG, "RESULT: %d", result);
#endif

  if (result != BME280_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  ether->state_machine.bme280 = BME280_STATE_INIT;

  return BME280_RESULT_SUCCESS;
}


bme280_result_t state_machine_bme280_id(ether_t *ether)
{
  if (!ether) {
    return BME280_RESULT_ERROR;
  }

  uint8_t data = 0;
  bme280_result_t result = bme280_id(ether->descriptor.i2c_controller.i2c_num, &data, sizeof(data));

#if defined(ETHER_DEBUG)
  ESP_LOGI(STATE_MACHINE_TAG, "BME280_STATE_ID");
  ESP_LOGI(STATE_MACHINE_TAG, "RESULT: %d", result);
#endif

  if ((result != BME280_RESULT_SUCCESS) || (data != BME280_DATA_ID)) {
    return BME280_RESULT_ERROR;
  }

  ether->state_machine.bme280 = BME280_STATE_GET_COMPENSATION_DATA;

  return BME280_RESULT_SUCCESS;
}


bme280_result_t state_machine_bme280_force_mode(ether_t *ether)
{
  if (!ether) {
    return BME280_RESULT_ERROR;
  }

  bme280_result_t result = bme280_force_mode(ether->descriptor.i2c_controller.i2c_num, &ether->settings.bme280);

#if defined(ETHER_DEBUG)
  ESP_LOGI(STATE_MACHINE_TAG, "BME280_STATE_FORCE_MODE");
  ESP_LOGI(STATE_MACHINE_TAG, "RESULT: %d", result);
#endif

  if (result != BME280_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  ether->state_machine.bme280 = BME280_STATE_MEASURE_HUMIDITY;

  return BME280_RESULT_SUCCESS;
}


bme280_result_t state_machine_bme280_measure_humidity(ether_t *ether)
{
  if (!ether) {
    return BME280_RESULT_ERROR;
  }

  bme280_result_t result = bme280_measure_humidity(ether->descriptor.i2c_controller.i2c_num, 
                                                   &ether->measurements.bme280.humidity);

#if defined(ETHER_DEBUG)
  ESP_LOGI(STATE_MACHINE_TAG, "BME280_STATE_MEASURE_HUMIDITY");
  ESP_LOGI(STATE_MACHINE_TAG, "RESULT: %d", result);
#endif

  if (result != BME280_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  ether->state_machine.bme280 = BME280_STATE_MEASURE_TEMPERATURE;

  return BME280_RESULT_SUCCESS;
}


bme280_result_t state_machine_bme280_measure_temperature(ether_t *ether)
{
  if (!ether) {
    return BME280_RESULT_ERROR;
  }

  bme280_result_t result = bme280_measure_temperature(ether->descriptor.i2c_controller.i2c_num, 
                                                      &ether->measurements.bme280.temperature);

#if defined(ETHER_DEBUG)
  ESP_LOGI(STATE_MACHINE_TAG, "BME280_STATE_MEASURE_TEMPERATURE");
  ESP_LOGI(STATE_MACHINE_TAG, "RESULT: %d", result);
#endif

  if (result != BME280_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  ether->state_machine.bme280 = BME280_STATE_MEASURE_PRESSURE;

  return BME280_RESULT_SUCCESS;
}


bme280_result_t state_machine_bme280_measure_pressure(ether_t *ether)
{
  if (!ether) {
    return BME280_RESULT_ERROR;
  }

  bme280_result_t result = bme280_measure_pressure(ether->descriptor.i2c_controller.i2c_num, 
                                                   &ether->measurements.bme280.pressure);

#if defined(ETHER_DEBUG)
  ESP_LOGI(STATE_MACHINE_TAG, "BME280_STATE_MEASURE_PRESSURE");
  ESP_LOGI(STATE_MACHINE_TAG, "RESULT: %d", result);
#endif

  if (result != BME280_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  ether->state_machine.bme280 = BME280_STATE_COMPENSATE_HUMIDITY;

  return BME280_RESULT_SUCCESS;
}


bme280_result_t state_machine_bme280_get_compensation_data(ether_t *ether)
{
  if (!ether) {
    return BME280_RESULT_ERROR;
  }

  bme280_result_t result = bme280_get_compensation_data(ether->descriptor.i2c_controller.i2c_num, 
                                                        &ether->measurements.bme280.compensator);

#if defined(ETHER_DEBUG)
  ESP_LOGI(STATE_MACHINE_TAG, "BME280_STATE_GET_COMPENSATION_DATA");
  ESP_LOGI(STATE_MACHINE_TAG, "RESULT: %d", result);
#endif

  if (result != BME280_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  ether->state_machine.bme280 = BME280_STATE_FORCE_MODE;

  return BME280_RESULT_SUCCESS;
}


bme280_result_t state_machine_bme280_compensate_humidity(ether_t *ether)
{
  if (!ether) {
    return BME280_RESULT_ERROR;
  }

  bme280_result_t result = bme280_compensate_humidity(&ether->measurements.bme280.compensator, 
                                                      &ether->measurements.bme280.humidity);

#if defined(ETHER_DEBUG)
  ESP_LOGI(STATE_MACHINE_TAG, "BME280_STATE_COMPENSATE_HUMIDITY");
  ESP_LOGI(STATE_MACHINE_TAG, "RESULT: %d", result);
#endif

  if (result != BME280_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  ether->state_machine.bme280 = BME280_STATE_COMPENSATE_TEMPERATURE;

  return BME280_RESULT_SUCCESS;
}


bme280_result_t state_machine_bme280_compensate_temperature(ether_t *ether)
{
  if (!ether) {
    return BME280_RESULT_ERROR;
  }

  bme280_result_t result = bme280_compensate_temperature(&ether->measurements.bme280.compensator, 
                                                         &ether->measurements.bme280.temperature);

#if defined(ETHER_DEBUG)
  ESP_LOGI(STATE_MACHINE_TAG, "BME280_STATE_COMPENSATE_TEMPERATURE");
  ESP_LOGI(STATE_MACHINE_TAG, "RESULT: %d", result);
#endif

  if (result != BME280_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  ether->state_machine.bme280 = BME280_STATE_COMPENSATE_PRESSURE;

  return BME280_RESULT_SUCCESS;
}


bme280_result_t state_machine_bme280_compensate_pressure(ether_t *ether)
{
  if (!ether) {
    return BME280_RESULT_ERROR;
  }

  bme280_result_t result = bme280_compensate_pressure(&ether->measurements.bme280.compensator, 
                                                      &ether->measurements.bme280.pressure);

#if defined(ETHER_DEBUG)
  ESP_LOGI(STATE_MACHINE_TAG, "BME280_STATE_COMPENSATE_PRESSURE");
  ESP_LOGI(STATE_MACHINE_TAG, "RESULT: %d", result);
#endif

  if (result != BME280_RESULT_SUCCESS) {
    return BME280_RESULT_ERROR;
  }

  ether->state_machine.bme280 = BME280_STATE_UNSET;

  return BME280_RESULT_SUCCESS;
}
