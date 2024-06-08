#ifndef INC_ETHER_H
#define INC_ETHER_H

#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "driver/uart.h"
#include "hal/uart_types.h"
#include "portmacro.h"
#include "string.h"
#include "driver/gpio.h"
#include "pms7003.h"
#include "bme280.h"
#include "i2c_controller.h"
#include "mqtt_client.h"
#include "uart_controller.h"
#include "wifi_controller.h"
#include "mqtt_controller.h"

/** 
 * \brief Result codes for ETHER operations.
 */
typedef enum {
  ETHER_RESULT_SUCCESS = 0,   /*!< Operation was successful. */
  ETHER_RESULT_ERROR,         /*!< Operation encountered an error. */
} ether_result_t;

/** 
 * \brief Structure to store PMS7003 and BME280 sensors measurements.
 */
typedef struct {
  pms7003_measurements_t pms7003;   /*!< PMS7003 measurements. */
  bme280_measurements_t bme280;     /*!< BME280 measurements. */
} ether_measurements_t;

/** 
 * \brief Structure to hold various controller descriptors.
 */
typedef struct {
  i2c_controller_descriptor_t i2c_controller;     /*!< I2C controller descriptor. */
  mqtt_controller_descriptor_t mqtt_controller;   /*!< MQTT controller descriptor. */
  uart_controller_descriptor_t uart_controller;   /*!< UART controller descriptor. */
  wifi_controller_descriptor_t wifi_controller;   /*!< WIFI controller descriptor. */
} ether_descriptor_t;

/** 
 * \brief Structure for ETHER settings.
 */
typedef struct {
  bme280_settings_t bme280;   /*!< BME280 sensor settings. */
} ether_settings_t;

/** 
 * \brief Structure to hold the state machine for PMS7003 and BME280 sensors.
 */
typedef struct {
  pms7003_state_t pms7003;  /*!< PMS7003 sensor state. */
  bme280_state_t bme280;    /*!< BME280 sensor state. */
} ether_state_machine_t;

/** 
 * \brief Main structure for ETHER containing measurements, descriptors, settings, and state machine.
 */
typedef struct {
  ether_measurements_t measurements;      /*!< Measurements data. */
  ether_descriptor_t descriptor;          /*!< Controller descriptors. */
  ether_settings_t settings;              /*!< Settings data. */
  ether_state_machine_t state_machine;    /*!< State machine data. */
} ether_t;

/**
 * \brief Initialize the ETHER system.
 * 
 * \param[out]  ether: Pointer to the ETHER structure.
 * \return      Result of the initialization.
 */
ether_result_t ether_init(ether_t *ether);

#endif // !INC_ETHER_H
