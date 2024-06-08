#include "ether.h"
#include "state_machine.h"

const TickType_t ether_delay_60s    = pdMS_TO_TICKS(60000);
const TickType_t ether_delay_30s    = pdMS_TO_TICKS(30000);
const TickType_t ether_delay_10s    = pdMS_TO_TICKS(10000);
const TickType_t ether_delay_1s     = pdMS_TO_TICKS(1000);
const TickType_t ether_delay_500ms  = pdMS_TO_TICKS(500);
const TickType_t ether_delay_200ms  = pdMS_TO_TICKS(200);
const TickType_t ether_delay_15ms   = pdMS_TO_TICKS(15);

SemaphoreHandle_t ether_pms7003_semaphore;
SemaphoreHandle_t ether_mqtt_semaphore; 
SemaphoreHandle_t ether_bme280_semaphore;

///////////////////////////////////////////////////////////////////////////////
/* BEGIN OF STATIC FUNCTIONS                                                 */
///////////////////////////////////////////////////////////////////////////////

/* The ether pointer can't be const because of the mqtt_descriptor. */
static void controller_init(ether_t *ether) 
{
  if (!ether) {
    return;
  }

  wifi_controller_init(&ether->descriptor.wifi_controller);
  vTaskDelay(ether_delay_1s);

  uart_controller_init(&ether->descriptor.uart_controller);
  vTaskDelay(ether_delay_1s);

  i2c_controller_init(&ether->descriptor.i2c_controller);
  vTaskDelay(ether_delay_1s);

  mqtt_controller_init(&ether->descriptor.mqtt_controller);
  vTaskDelay(ether_delay_1s);
}

static uint16_t convert_to_little_endian(uint16_t data) 
{
  return ((data & 0x00ff) << 8 | (data & 0xff00) >> 8);
}

static void create_mqtt_message(const ether_t *ether, char *mqtt_message)
{
  if ((!ether) || (!mqtt_message)) {
    return;
  }

  snprintf(mqtt_message, MQTT_CONTROLLER_MESSAGE_MAX_SIZE, 
           "ether measurements:\n\rpm1 = %d\n\rpm2.5 = %d\n\rpm10 = %d\n\rtemp = %f\n\rhum = %f\n\rpress = %f\n\r", 
           ether->measurements.pms7003.pm1, 
           ether->measurements.pms7003.pm25, 
           ether->measurements.pms7003.pm10,
           ether->measurements.bme280.temperature.compensated,
           ether->measurements.bme280.humidity.compensated,
           ether->measurements.bme280.pressure.compensated);
}

///////////////////////////////////////////////////////////////////////////////
/* END OF STATIC FUNCTIONS                                                   */
///////////////////////////////////////////////////////////////////////////////

void ether_mqtt_task(void *arg)
{
  
  static const char *MQTT_TASK_TAG = "MQTT_TASK";
  esp_log_level_set(MQTT_TASK_TAG, ESP_LOG_INFO);

  if (!arg) {
    ESP_LOGE(MQTT_TASK_TAG, "Received null pointer argument");
    vTaskDelete(xTaskGetCurrentTaskHandle());
    return;
  }

  ether_t *ether = arg;
  char mqtt_message[MQTT_CONTROLLER_MESSAGE_MAX_SIZE];
  const char *mqtt_topic = "/topic/ether";
  int result = 0;

  while (1) {
    xSemaphoreTake(ether_mqtt_semaphore, portMAX_DELAY);

#if defined(ETHER_DEBUG)
    ESP_LOGI(MQTT_TASK_TAG, "create data:");
#endif

    create_mqtt_message(ether, mqtt_message);

#if defined(ETHER_DEBUG)
    ESP_LOGI(MQTT_TASK_TAG, "send data:");
#endif

    result = esp_mqtt_client_publish(ether->descriptor.mqtt_controller.client_handle, 
                                     mqtt_topic, mqtt_message, 0, 0, 0);

#if defined(ETHER_DEBUG)
    ESP_LOGI(MQTT_TASK_TAG, "result: %d", result);
#endif

    vTaskDelay(ether_delay_60s);
    xSemaphoreGive(ether_pms7003_semaphore);
  }
}

void ether_bme280_task(void *arg)
{
  static const char *BME280_TASK_TAG = "BME280_TASK";
  esp_log_level_set(BME280_TASK_TAG, ESP_LOG_INFO);

  if (!arg) {
    ESP_LOGE(BME280_TASK_TAG, "Received null pointer argument");
    vTaskDelete(xTaskGetCurrentTaskHandle());
    return;
  }

  ether_t *ether = arg;
  uint8_t retry = 0;
  bme280_result_t result;

  ether->state_machine.bme280 = BME280_STATE_RESET;

  while (1) {
    xSemaphoreTake(ether_bme280_semaphore, portMAX_DELAY);

    while ((ether->state_machine.bme280 != BME280_STATE_UNSET) && (retry < 5)) {
      switch (ether->state_machine.bme280) {
        case BME280_STATE_RESET: {
          result = state_machine_bme280_reset(ether);
          if (result != BME280_RESULT_SUCCESS) {
            ++retry;
          }
          break;
        }
        case BME280_STATE_INIT: {
          result = state_machine_bme280_init(ether);
          if (result != BME280_RESULT_SUCCESS) {
            ++retry;
          }
          break;
        }
        case BME280_STATE_ID: {
          result = state_machine_bme280_id(ether);
          if (result != BME280_RESULT_SUCCESS) {
            ++retry;
          }
          break;
        }
        case BME280_STATE_GET_COMPENSATION_DATA: {
          result = state_machine_bme280_get_compensation_data(ether);
          if (result != BME280_RESULT_SUCCESS) {
            ++retry;
          }
          break;
        }
        case BME280_STATE_FORCE_MODE: {
          result = state_machine_bme280_force_mode(ether);
          if (result != BME280_RESULT_SUCCESS) {
            ++retry;
            break;
          }
          /* 
           * time_measure[max_in_ms] = 1.25 + (2.3 * temp_oversampling) + 
           *                           (2.3 * press_oversampling + 0.575) +
           *                           (2.3 * hum_oversampling + 0.575)
           *
           * time_measure[max_in_ms] = 1.25 + 2.3 + (2.3 + 0.575) + (2.3 + 0.575)
           * time_measure[max_in_ms] = 9.3ms
           */
          vTaskDelay(ether_delay_15ms);
          break;
        }
        case BME280_STATE_MEASURE_HUMIDITY: {
          result = state_machine_bme280_measure_humidity(ether);
          if (result != BME280_RESULT_SUCCESS) {
            ++retry;
          }
          break;
        }
        case BME280_STATE_MEASURE_TEMPERATURE: {
          result = state_machine_bme280_measure_temperature(ether);
          if (result != BME280_RESULT_SUCCESS) {
            ++retry;
          }
          break;
        }
        case BME280_STATE_MEASURE_PRESSURE: {
          result = state_machine_bme280_measure_pressure(ether);
          if (result != BME280_RESULT_SUCCESS) {
            ++retry;
          }
          break;
        }
        case BME280_STATE_COMPENSATE_HUMIDITY: {
          result = state_machine_bme280_compensate_humidity(ether);
          if (result != BME280_RESULT_SUCCESS) {
            ++retry;
          }
          break;
        }
        case BME280_STATE_COMPENSATE_TEMPERATURE: {
          result = state_machine_bme280_compensate_temperature(ether);
          if (result != BME280_RESULT_SUCCESS) {
            ++retry;
          }
          break;
        }
        case BME280_STATE_COMPENSATE_PRESSURE: {
          result = state_machine_bme280_compensate_pressure(ether);
          if (result != BME280_RESULT_SUCCESS) {
            ++retry;
          }
          break;
        }
        default: {
          ether->state_machine.bme280 = BME280_STATE_UNSET;
          break;
        }
      }
    }

    ether->state_machine.bme280 = BME280_STATE_FORCE_MODE;
    retry = 0;

#if defined(ETHER_DEBUG)
    ESP_LOGI(BME280_TASK_TAG, "humidity = %f", ether->measurements.bme280.humidity.compensated);
    ESP_LOGI(BME280_TASK_TAG, "pressure = %f", ether->measurements.bme280.pressure.compensated);
    ESP_LOGI(BME280_TASK_TAG, "temperature = %f\n\r", ether->measurements.bme280.temperature.compensated);
#endif

    vTaskDelay(ether_delay_500ms);

    xSemaphoreGive(ether_mqtt_semaphore);
  }
}

void ether_pms7003_task(void *arg)
{
  static const char *PMS7003_TASK_TAG = "PMS7003_TASK";
  esp_log_level_set(PMS7003_TASK_TAG, ESP_LOG_INFO);

  if (!arg) {
    ESP_LOGE(PMS7003_TASK_TAG, "Received null pointer argument");
    vTaskDelete(xTaskGetCurrentTaskHandle());
    return;
  }

  ether_t *ether = arg;
  pms7003_frame_answer_t frame = { 0 };
  uint8_t retry = 0;
  pms7003_result_t result;

  ether->state_machine.pms7003 = PMS7003_STATE_CHANGE_MODE_PASSIVE;

  while (1) {
    xSemaphoreTake(ether_pms7003_semaphore, portMAX_DELAY);
    
    while ((ether->state_machine.pms7003 != PMS7003_STATE_UNSET) && (retry < 5)) {
      switch (ether->state_machine.pms7003) {
        case PMS7003_STATE_CHANGE_MODE_PASSIVE: {
          result = pms7003_frame_send(&pms7003_change_mode_passive,
                                      ether->descriptor.uart_controller.uart_port);

#if defined(ETHER_DEBUG)
          ESP_LOGI(PMS7003_TASK_TAG, "PMS7003_STATE_CHANGE_MODE_PASSIVE");
          ESP_LOGI(PMS7003_TASK_TAG, "RESULT: %d", result);
#endif

          if (result != PMS7003_RESULT_SUCCESS) {
            ++retry;
            break;
          }

          ether->state_machine.pms7003 = PMS7003_STATE_WAKEUP;
          vTaskDelay(ether_delay_500ms);
          break;
        }
        case PMS7003_STATE_CHANGE_MODE_ACTIVE: {
          result = pms7003_frame_send(&pms7003_change_mode_active,
                                      ether->descriptor.uart_controller.uart_port);

#if defined(ETHER_DEBUG)
          ESP_LOGI(PMS7003_TASK_TAG, "PMS7003_STATE_CHANGE_MODE_ACTIVE");
          ESP_LOGI(PMS7003_TASK_TAG, "RESULT: %d", result);
#endif
          if (result != PMS7003_RESULT_SUCCESS) {
            ++retry;
            break;
          }

          ether->state_machine.pms7003 = PMS7003_STATE_WAKEUP;
          vTaskDelay(ether_delay_500ms);
          break;
        }
        case PMS7003_STATE_WAKEUP: {
          result = pms7003_frame_send(&pms7003_wakeup, ether->descriptor.uart_controller.uart_port);

#if defined(ETHER_DEBUG)
          ESP_LOGI(PMS7003_TASK_TAG, "PMS7003_STATE_WAKEUP");
          ESP_LOGI(PMS7003_TASK_TAG, "RESULT: %d", result);
#endif
          if (result != PMS7003_RESULT_SUCCESS) {
            ++retry;
            break;
          }

          ether->state_machine.pms7003 = PMS7003_STATE_READ_REQUEST;
          /* Wait at least 30s to get stable data. */
          vTaskDelay(ether_delay_30s);
          break;
        }
        case PMS7003_STATE_READ_REQUEST: {
          for (uint8_t i = 0; i < 5; i++) {
            /* Avoid getting unstable data. */
            uart_flush(ether->descriptor.uart_controller.uart_port);
            result = pms7003_frame_send(&pms7003_read_request, ether->descriptor.uart_controller.uart_port);
            vTaskDelay(ether_delay_500ms);
          }

#if defined(ETHER_DEBUG)
          ESP_LOGI(PMS7003_TASK_TAG, "PMS7003_STATE_READ_REQUEST");
          ESP_LOGI(PMS7003_TASK_TAG, "RESULT: %d", result);
#endif

          if (result != PMS7003_RESULT_SUCCESS) {
            ++retry;
            break;
          }

          ether->state_machine.pms7003 = PMS7003_STATE_READ;
          break;
        }
        case PMS7003_STATE_READ: {
          result = pms7003_frame_receive(&pms7003_read, ether->descriptor.uart_controller.uart_port, &frame);

#if defined(ETHER_DEBUG)
          ESP_LOGI(PMS7003_TASK_TAG, "PMS7003_STATE_READ");
          ESP_LOGI(PMS7003_TASK_TAG, "RESULT: %d", result);
#endif

          if (result != PMS7003_RESULT_SUCCESS) {
            ++retry;
            break;
          }

          ether->state_machine.pms7003 = PMS7003_STATE_SLEEP;
          vTaskDelay(ether_delay_500ms);
          break;
        }
        case PMS7003_STATE_SLEEP: {
          result = pms7003_frame_send(&pms7003_sleep, ether->descriptor.uart_controller.uart_port);

#if defined(ETHER_DEBUG)
          ESP_LOGI(PMS7003_TASK_TAG, "PMS7003_STATE_SLEEP");
          ESP_LOGI(PMS7003_TASK_TAG, "RESULT: %d", result);
#endif

          if (result != PMS7003_RESULT_SUCCESS) {
            ++retry;
            break;
          }

          ether->state_machine.pms7003 = PMS7003_STATE_UNSET;
          vTaskDelay(ether_delay_500ms);
          break;
        }
        default: {
#if defined(ETHER_DEBUG)
          ESP_LOGI(PMS7003_TASK_TAG, "default");
#endif
          ether->state_machine.pms7003 = PMS7003_STATE_UNSET;
          vTaskDelay(ether_delay_500ms);
          break;
        }
      }
    }
    
    ether->measurements.pms7003.pm1   = convert_to_little_endian(frame.data_pm1_standard);
    ether->measurements.pms7003.pm25  = convert_to_little_endian(frame.data_pm25_standard);
    ether->measurements.pms7003.pm10  = convert_to_little_endian(frame.data_pm10_standard);

    ether->state_machine.pms7003 = PMS7003_STATE_WAKEUP;

    retry = 0;

    xSemaphoreGive(ether_bme280_semaphore);
  }
}

void app_main(void) 
{
  ether_t ether;

  esp_task_wdt_deinit();

  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ether_init(&ether);
  controller_init(&ether);

  ether_pms7003_semaphore = xSemaphoreCreateBinary();
  ether_mqtt_semaphore    = xSemaphoreCreateBinary();
  ether_bme280_semaphore  = xSemaphoreCreateBinary();

  xSemaphoreGive(ether_pms7003_semaphore);

  xTaskCreate(ether_pms7003_task, "pms7003_task", 4096 * 2, &ether, configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(ether_mqtt_task, "mqtt_task", 4096 * 2, &ether, configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(ether_bme280_task, "bme280_task", 4096 * 2, &ether, configMAX_PRIORITIES - 1, NULL);

  while(1);
}
