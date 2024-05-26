#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
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
#include <stdio.h>

#define MQTT_MESSAGE_MAX_SIZE (256)

SemaphoreHandle_t pms7003_semaphore, mqtt_semaphore, bme280_semaphore;

const TickType_t delay_60s    = pdMS_TO_TICKS(60000);
const TickType_t delay_10s    = pdMS_TO_TICKS(10000);
const TickType_t delay_1s     = pdMS_TO_TICKS(1000);
const TickType_t delay_500ms  = pdMS_TO_TICKS(500);
const TickType_t delay_200ms  = pdMS_TO_TICKS(200);
const TickType_t delay_15ms   = pdMS_TO_TICKS(15);

typedef struct {
  pms7003_frame_answer_t  pms7003_frame_answer;
  bme280_measurements_t   bme280_measurements;
} ether_measurements_t;

static void ether_init(void) 
{
  wifi_controller_init(&wifi_controller_descriptor_default);
  vTaskDelay(delay_1s);
  uart_controller_init(&uart_controller_descriptor_default);
  vTaskDelay(delay_1s);
  i2c_controller_init(&i2c_controller_descriptor_default);
  vTaskDelay(delay_1s);
  mqtt_controller_init(&mqtt_controller_descriptor_default);
  vTaskDelay(delay_1s);
}

static uint16_t convert_to_little_endian(uint16_t data) 
{
  return ((data & 0x00ff) << 8 | (data & 0xff00) >> 8);
}

static void create_mqtt_message(const ether_measurements_t *ether_measurements, 
                                char *mqtt_message)
{
  // if ((!ether_measurements) || (!mqtt_message)) {
  //   return;
  // }

  snprintf(mqtt_message, MQTT_MESSAGE_MAX_SIZE, 
           "ether measurements:\n\rpm1 = %d\n\rpm2.5 = %d\n\rpm10 = %d\n\rtemp = %f\n\rhum = %f\n\rpress = %f\n\r", 
           convert_to_little_endian(ether_measurements->pms7003_frame_answer.data_pm1_standard), 
           convert_to_little_endian(ether_measurements->pms7003_frame_answer.data_pm25_standard), 
           convert_to_little_endian(ether_measurements->pms7003_frame_answer.data_pm10_standard),
           ether_measurements->bme280_measurements.temperature.compensated,
           ether_measurements->bme280_measurements.humidity.compensated,
           ether_measurements->bme280_measurements.pressure.compensated);

  printf("%s\n\r", mqtt_message);
}

static void mqtt_task(void *arg) 
{
  if (!arg) {
    ESP_LOGE("MQTT_TASK", "Received null pointer argument");
    vTaskDelete(xTaskGetCurrentTaskHandle());
    return;
  }

  ether_measurements_t *ether_measurements = (ether_measurements_t*)arg;
  char mqtt_message[MQTT_MESSAGE_MAX_SIZE];
  const char *mqtt_topic = "/topic/ether";
  int result = 0;

  static const char *MQTT_TASK_TAG = "MQTT_TASK";
  esp_log_level_set(MQTT_TASK_TAG, ESP_LOG_INFO);

  while (1) {
    xSemaphoreTake(mqtt_semaphore, portMAX_DELAY);

    ESP_LOGI(MQTT_TASK_TAG, "create data:");

    create_mqtt_message(ether_measurements, 
                        mqtt_message);

    ESP_LOGI(MQTT_TASK_TAG, "send data:");

    result = esp_mqtt_client_publish(mqtt_controller_descriptor_default.client_handle, 
                            mqtt_topic, mqtt_message, 0, 0, 0);

    ESP_LOGI(MQTT_TASK_TAG, "result: %d", result);

    vTaskDelay(delay_60s);
    xSemaphoreGive(pms7003_semaphore);
  }
}

static void bme280_task(void *arg) 
{
  if (!arg) {
    ESP_LOGE("BME280_TASK", "Received null pointer argument");
    vTaskDelete(xTaskGetCurrentTaskHandle());
    return;
  }

  static const char *BME280_TASK_TAG = "BME280_TASK";
  esp_log_level_set(BME280_TASK_TAG, ESP_LOG_INFO);

  bme280_measurements_t *bme280_measurements = arg;

  uint8_t data = 0;
  
  bme280_reset(i2c_controller_descriptor_default.i2c_num);
  bme280_init(i2c_controller_descriptor_default.i2c_num, &bme280_default_settings);
  bme280_id(i2c_controller_descriptor_default.i2c_num, &data, sizeof(data));

  if (data != BME280_DATA_ID) {
    ESP_LOGI(BME280_TASK_TAG, "The id is not valid!\n\rClosing bme280 task...\n\r");
    vTaskDelete(xTaskGetCurrentTaskHandle());
  }

  bme280_get_compensation_data(i2c_controller_descriptor_default.i2c_num, 
                               &bme280_measurements->compensator);

  while (1) {
    xSemaphoreTake(bme280_semaphore, portMAX_DELAY);

    bme280_force_mode(i2c_controller_descriptor_default.i2c_num, &bme280_default_settings);

    /* 
     * time_measure[max_in_ms] = 1.25 + (2.3 * temp_oversampling) + 
     *                           (2.3 * press_oversampling + 0.575) +
     *                           (2.3 * hum_oversampling + 0.575)
     *
     * time_measure[max_in_ms] = 1.25 + 2.3 + (2.3 + 0.575) + (2.3 + 0.575)
     * time_measure[max_in_ms] = 9.3ms
     */
    vTaskDelay(delay_15ms);

    bme280_measure_humidity(i2c_controller_descriptor_default.i2c_num, 
                            &bme280_measurements->humidity);

    bme280_measure_pressure(i2c_controller_descriptor_default.i2c_num, 
                            &bme280_measurements->pressure);

    bme280_measure_temperature(i2c_controller_descriptor_default.i2c_num, 
                               &bme280_measurements->temperature);

    bme280_compensate_humidity(&bme280_measurements->compensator, 
                               &bme280_measurements->humidity);

    bme280_compensate_pressure(&bme280_measurements->compensator, 
                               &bme280_measurements->pressure);

    bme280_compensate_temperature(&bme280_measurements->compensator, 
                                  &bme280_measurements->temperature);
    
    ESP_LOGI(BME280_TASK_TAG, "humidity = %f", 
             bme280_measurements->humidity.compensated);

    ESP_LOGI(BME280_TASK_TAG, "pressure = %f", 
             bme280_measurements->pressure.compensated);

    ESP_LOGI(BME280_TASK_TAG, "temperature = %f\n\r", 
             bme280_measurements->temperature.compensated);

    vTaskDelay(delay_500ms);

    xSemaphoreGive(mqtt_semaphore);
  }
}

static void pms7003_task(void *arg)
{
  static const char *PMS7003_TASK_TAG = "PMS7003_TASK";
  esp_log_level_set(PMS7003_TASK_TAG, ESP_LOG_INFO);

  if (!arg) {
    ESP_LOGE(PMS7003_TASK_TAG, "Received null pointer argument");
    vTaskDelete(xTaskGetCurrentTaskHandle());
    return;
  }

  pms7003_frame_answer_t *pms7003_frame_answer = (pms7003_frame_answer_t*)arg;

  pms7003_frame_send(&pms7003_change_mode_passive, 
                     uart_controller_descriptor_default.uart_port);

  vTaskDelay(delay_500ms);

  while (1) {
    xSemaphoreTake(pms7003_semaphore, portMAX_DELAY);
    
    pms7003_frame_send(&pms7003_wakeup, uart_controller_descriptor_default.uart_port);

    /* Wait at least 30s to get stable data. */
    vTaskDelay(delay_60s/2);

    for (uint8_t i = 0; i < 5; i++) {
      /* Avoid getting not stable data. */
      uart_flush(uart_controller_descriptor_default.uart_port);
      pms7003_frame_send(&pms7003_read_request, uart_controller_descriptor_default.uart_port);
      vTaskDelay(delay_500ms);
    }
    
    pms7003_frame_receive(&pms7003_read, 
                          uart_controller_descriptor_default.uart_port, pms7003_frame_answer);

    vTaskDelay(delay_500ms);

    pms7003_frame_send(&pms7003_sleep, uart_controller_descriptor_default.uart_port);

    vTaskDelay(delay_500ms);

    xSemaphoreGive(bme280_semaphore);
  }
}

void app_main(void) 
{
  ether_measurements_t ether_measurements = { 0 };

  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ether_init();

  pms7003_semaphore = xSemaphoreCreateBinary();
  mqtt_semaphore    = xSemaphoreCreateBinary();
  bme280_semaphore  = xSemaphoreCreateBinary();

  xSemaphoreGive(pms7003_semaphore);

  xTaskCreate(pms7003_task, "pms7003_task", 1024 * 2, &ether_measurements.pms7003_frame_answer, 
              configMAX_PRIORITIES - 1, NULL);

  xTaskCreate(mqtt_task, "mqtt_task", 1024 * 2, &ether_measurements, 
              configMAX_PRIORITIES - 1, NULL);

  xTaskCreate(bme280_task, "bme280_task", 1024 * 2, &ether_measurements.bme280_measurements, 
              configMAX_PRIORITIES - 1, NULL);
}
