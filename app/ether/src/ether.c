#include "ether.h"

ether_result_t ether_init(ether_t *ether)
{
  if (!ether) {
    return ETHER_RESULT_ERROR;
  }

  /* 
   * Faster way:
   * ether_t ether = { 0 };
   *
   * ether_init(&ether);
   *
   * but it initializes only descriptors.
   *
   * MADE FOR FUN.
   */
  ether->measurements.pms7003.pm1  = 0;
  ether->measurements.pms7003.pm25 = 0;
  ether->measurements.pms7003.pm10 = 0;

  ether->measurements.bme280.humidity.msb = 0;
  ether->measurements.bme280.humidity.lsb = 0;
  ether->measurements.bme280.humidity.compensated = 0;

  ether->measurements.bme280.pressure.msb = 0;
  ether->measurements.bme280.pressure.lsb = 0;
  ether->measurements.bme280.pressure.xlsb = 0;
  ether->measurements.bme280.pressure.compensated = 0;

  ether->measurements.bme280.temperature.msb = 0;
  ether->measurements.bme280.temperature.lsb = 0;
  ether->measurements.bme280.temperature.xlsb = 0;
  ether->measurements.bme280.temperature.compensated = 0;

  ether->measurements.bme280.compensator.dig_t1 = 0;
  ether->measurements.bme280.compensator.dig_t2 = 0;
  ether->measurements.bme280.compensator.dig_t3 = 0;
  ether->measurements.bme280.compensator.dig_p1 = 0;
  ether->measurements.bme280.compensator.dig_p2 = 0;
  ether->measurements.bme280.compensator.dig_p3 = 0;
  ether->measurements.bme280.compensator.dig_p4 = 0;
  ether->measurements.bme280.compensator.dig_p5 = 0;
  ether->measurements.bme280.compensator.dig_p6 = 0;
  ether->measurements.bme280.compensator.dig_p7 = 0;
  ether->measurements.bme280.compensator.dig_p8 = 0;
  ether->measurements.bme280.compensator.dig_p9 = 0;
  ether->measurements.bme280.compensator.dig_h1 = 0;
  ether->measurements.bme280.compensator.dig_h2 = 0;
  ether->measurements.bme280.compensator.dig_h3 = 0;
  ether->measurements.bme280.compensator.dig_h4 = 0;
  ether->measurements.bme280.compensator.dig_h5 = 0;
  ether->measurements.bme280.compensator.dig_h6 = 0;

  ether->descriptor.i2c_controller  = (i2c_controller_descriptor_t)I2C_CONTROLLER_DESCRIPTOR_DEFAULT;
  ether->descriptor.mqtt_controller = (mqtt_controller_descriptor_t)MQTT_CONTROLLER_DESCRIPTOR_DEFAULT;
  ether->descriptor.uart_controller = (uart_controller_descriptor_t)UART_CONTROLLER_DESCRIPTOR_DEFAULT;
  ether->descriptor.wifi_controller = (wifi_controller_descriptor_t)WIFI_CONTROLLER_DESCRIPTOR_DEFAULT;

  ether->settings.bme280 = (bme280_settings_t)BME280_SETTINGS_DEFAULT;

  ether->state_machine.bme280 = BME280_STATE_UNSET;
  ether->state_machine.pms7003 = PMS7003_STATE_UNSET;

  return ETHER_RESULT_SUCCESS;
}

