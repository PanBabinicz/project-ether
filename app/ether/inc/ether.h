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

typedef enum {
  ETHER_RESULT_SUCCESS = 0,
  ETHER_RESULT_ERROR,
} ether_result_t;

typedef struct {
  pms7003_measurements_t pms7003;
  bme280_measurements_t bme280;
} ether_measurements_t;

typedef struct {
  i2c_controller_descriptor_t i2c_controller;
  mqtt_controller_descriptor_t mqtt_controller;
  uart_controller_descriptor_t uart_controller;
  wifi_controller_descriptor_t wifi_controller;
} ether_descriptor_t;

typedef struct {
  bme280_settings_t bme280;
} ether_settings_t;

typedef struct {
  pms7003_state_t pms7003;
  bme280_state_t bme280;
} ether_state_machine_t;

typedef struct {
  ether_measurements_t measurements;
  ether_descriptor_t descriptor;
  ether_settings_t settings;
  ether_state_machine_t state_machine;
} ether_t;

ether_result_t ether_init(ether_t *ether);

#endif // !INC_ETHER_H
