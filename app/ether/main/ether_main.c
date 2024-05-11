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

SemaphoreHandle_t wake_up_semaphore, sleep_semaphore, rx_semaphore, tx_semaphore, print_semaphore, bme280_semaphore;

const TickType_t delay_60s = pdMS_TO_TICKS(60000);
const TickType_t delay_10s = pdMS_TO_TICKS(10000);
const TickType_t delay_1s = pdMS_TO_TICKS(1000);
const TickType_t delay_500ms = pdMS_TO_TICKS(500);
const TickType_t delay_200ms = pdMS_TO_TICKS(200);

void init(void) {
  wifi_controller_init(&wifi_controller_descriptor_default);
  vTaskDelay(delay_1s);
  uart_controller_init(&uart_controller_descriptor_default);
  vTaskDelay(delay_1s);
  i2c_controller_init(&i2c_controller_descriptor_default);
  vTaskDelay(delay_1s);
  mqtt_controller_init(&mqtt_controller_descriptor_default);
  vTaskDelay(delay_1s);
}

static uint16_t convert_to_little_endian(uint16_t data) {
  return ((data & 0x00ff) << 8 | (data & 0xff00) >> 8);
}

static void tx_task(void *arg) {

  static const char *TX_TASK_TAG = "TX_TASK";
  esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
  while (1) {
    xSemaphoreTake(tx_semaphore, portMAX_DELAY);

    vTaskDelay(5*delay_500ms);

    for (int i = 0; i < 5; ++i) {
      uart_flush(uart_controller_descriptor_default.uart_port);
      pms7003_frame_send(pms7003_read_request, uart_controller_descriptor_default.uart_port);
      vTaskDelay(delay_500ms);
    }

    xSemaphoreGive(rx_semaphore);
  }
}

static void rx_task(void *arg) {
  if (!arg) {
    ESP_LOGE("RX_TASK", "Received null pointer argument");
    vTaskDelete(xTaskGetCurrentTaskHandle());
    return;
  }

  pms7003_frame_answer_t *frame = (pms7003_frame_answer_t*)arg;

  static const char *RX_TASK_TAG = "RX_TASK";
  esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);

  while (1) {
    xSemaphoreTake(rx_semaphore, portMAX_DELAY);

    pms7003_frame_receive(pms7003_read, uart_controller_descriptor_default.uart_port, frame); 

    vTaskDelay(delay_500ms);

    xSemaphoreGive(sleep_semaphore);
  }
}

static void print_measurements_task(void *arg) {
  if (!arg) {
    ESP_LOGE("PRINT_MEASUREMENTS_TASK", "Received null pointer argument");
    vTaskDelete(xTaskGetCurrentTaskHandle());
    return;
  }

  pms7003_frame_answer_t *frame = (pms7003_frame_answer_t*)arg;

  static const char *PRINT_MEASUREMENTS_TASK_TAG = "PRINT_MEASUREMENTS_TASK";
  esp_log_level_set(PRINT_MEASUREMENTS_TASK_TAG, ESP_LOG_INFO);

  while (1) {
    xSemaphoreTake(print_semaphore, portMAX_DELAY);

    ESP_LOGI(PRINT_MEASUREMENTS_TASK_TAG, "MEASUREMENTS:");
    ESP_LOGI(PRINT_MEASUREMENTS_TASK_TAG, "PM1_STANDARD = %d", convert_to_little_endian(frame->data_pm1_standard));
    ESP_LOGI(PRINT_MEASUREMENTS_TASK_TAG, "PM25_STANDARD = %d", convert_to_little_endian(frame->data_pm25_standard));
    ESP_LOGI(PRINT_MEASUREMENTS_TASK_TAG, "PM10_STANDARD = %d", convert_to_little_endian(frame->data_pm10_standard));

    ESP_LOG_BUFFER_HEXDUMP(PRINT_MEASUREMENTS_TASK_TAG, frame->buffer_answer, PMS7003_FRAME_ANSWER_SIZE, ESP_LOG_INFO);

    // vTaskDelay(delay_60s);
    vTaskDelay(delay_500ms);
    xSemaphoreGive(bme280_semaphore);
  }
}

static void sleep_task(void *arg) {
  static const char *SLEEP_TASK_TAG = "SLEEP_TASK";
  esp_log_level_set(SLEEP_TASK_TAG, ESP_LOG_INFO);

  while (1) {
    xSemaphoreTake(sleep_semaphore, portMAX_DELAY);
    
    pms7003_frame_send(pms7003_sleep, uart_controller_descriptor_default.uart_port);

    vTaskDelay(delay_500ms);

    xSemaphoreGive(print_semaphore);
  }
}

static void wake_up_task(void *arg) {
  static const char *WAKE_UP_TASK_TAG = "WAKE_UP_TASK";
  esp_log_level_set(WAKE_UP_TASK_TAG, ESP_LOG_INFO);

  while (1) {
    xSemaphoreTake(wake_up_semaphore, portMAX_DELAY);

    pms7003_frame_send(pms7003_wakeup, uart_controller_descriptor_default.uart_port);

    vTaskDelay(delay_500ms);

    xSemaphoreGive(tx_semaphore);
  }
}

static void bme280_task(void *arg) {
  if (!arg) {
    ESP_LOGE("BME280_TASK", "Received null pointer argument");
    vTaskDelete(xTaskGetCurrentTaskHandle());
    return;
  }

  static const char *BME280_TASK_TAG = "BME280_TASK";
  esp_log_level_set(BME280_TASK_TAG, ESP_LOG_INFO);

  bme280_measurements_t *bme280_measurements = (bme280_measurements_t*)arg;

  uint8_t data = 0;
  
  bme280_reset(i2c_controller_descriptor_default.i2c_num);
  bme280_init(i2c_controller_descriptor_default.i2c_num, &bme280_default_settings);
  bme280_id(i2c_controller_descriptor_default.i2c_num, &data, sizeof(data));

  if (data != BME280_DATA_ID) {
    ESP_LOGI(BME280_TASK_TAG, "The id is not valid!\n\rClosing bme280 task...\n\r");
    vTaskDelete(xTaskGetCurrentTaskHandle());
  }

  bme280_get_compensation_data(i2c_controller_descriptor_default.i2c_num, &bme280_measurements->compensator);

  while (1) {
    xSemaphoreTake(bme280_semaphore, portMAX_DELAY);

    bme280_force_mode(i2c_controller_descriptor_default.i2c_num, &bme280_default_settings);
    vTaskDelay(delay_500ms);

    bme280_measure_humidity(i2c_controller_descriptor_default.i2c_num, &bme280_measurements->humidity);
    bme280_measure_pressure(i2c_controller_descriptor_default.i2c_num, &bme280_measurements->pressure);
    bme280_measure_temperature(i2c_controller_descriptor_default.i2c_num, &bme280_measurements->temperature);

    bme280_compensate_humidity(&bme280_measurements->compensator, &bme280_measurements->humidity);
    bme280_compensate_pressure(&bme280_measurements->compensator, &bme280_measurements->pressure);
    bme280_compensate_temperature(&bme280_measurements->compensator, &bme280_measurements->temperature);
    
    ESP_LOGI(BME280_TASK_TAG, "humidity = %f\n\r", bme280_measurements->humidity.compensated);
    ESP_LOGI(BME280_TASK_TAG, "pressure = %f\n\r", bme280_measurements->pressure.compensated);
    ESP_LOGI(BME280_TASK_TAG, "temperature = %f\n\r", bme280_measurements->temperature.compensated);

    vTaskDelay(delay_60s);

    xSemaphoreGive(wake_up_semaphore);
  }
}

void app_main(void) {
  pms7003_frame_answer_t pms7003_frame = { 0 };
  bme280_measurements_t bme280_measurements = { 0 };

  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  init();

  pms7003_frame_send(pms7003_change_mode_passive, uart_controller_descriptor_default.uart_port);

  sleep_semaphore = xSemaphoreCreateBinary();
  wake_up_semaphore = xSemaphoreCreateBinary();
  rx_semaphore = xSemaphoreCreateBinary();
  tx_semaphore = xSemaphoreCreateBinary();
  print_semaphore = xSemaphoreCreateBinary();
  bme280_semaphore = xSemaphoreCreateBinary();

  xSemaphoreGive(wake_up_semaphore);
  // xSemaphoreGive(bme280_semaphore);

  xTaskCreate(wake_up_task, "wake_up_task", 1024 * 2, NULL, configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(sleep_task, "sleep_task", 1024 * 2, NULL, configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(rx_task, "uart_rx_task", 1024 * 2, &pms7003_frame, configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(tx_task, "uart_tx_task", 1024 * 2, NULL, configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(print_measurements_task, "print_measurements_task", 1024 * 2, &pms7003_frame, configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(bme280_task, "bme280_task", 1024 * 2, &bme280_measurements, configMAX_PRIORITIES - 1, NULL);
}
