#include "wifi_controller.h"
/* FreeRTOS event group to signal when we are connected */
static EventGroupHandle_t event_group;

const wifi_controller_descriptor_t wifi_controller_descriptor_default = WIFI_CONTROLLER_DESCRIPTOR_DEFAULT;

void wifi_controller_event_handler(void *arg, esp_event_base_t event_base, 
                                   int32_t event_id, void *event_data) 
{
  const char *TAG = "WIFI_CONTROLLER_EVENT_HANDLER";
  uint8_t current_retry_num = 0;

  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    if (current_retry_num < WIFI_CONTROLLER_RETRY_MAX) {
      esp_wifi_connect();
      current_retry_num++;
      ESP_LOGI(TAG, "retry to connect to the AP");
    } else {
      xEventGroupSetBits(event_group, WIFI_CONTROLLER_BIT_FAIL);
    }
    ESP_LOGI(TAG,"connect to the AP fail");
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    current_retry_num = 0;
    xEventGroupSetBits(event_group, WIFI_CONTROLLER_BIT_CONNECTED);
  }
}

wifi_controller_result_t wifi_controller_init(const wifi_controller_descriptor_t *wifi_controller_descriptor) 
{
  const char *TAG = "WIFI_CONTROLLER_INIT";

  if (!wifi_controller_descriptor) {
    return WIFI_CONTROLLER_RESULT_ERROR;
  }

  event_group = xEventGroupCreate();

  ESP_ERROR_CHECK(esp_netif_init());

  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  esp_event_handler_instance_t instance_any_id;
  esp_event_handler_instance_t instance_got_ip;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                      ESP_EVENT_ANY_ID,
                                                      wifi_controller_descriptor->event_handler,
                                                      NULL,
                                                      &instance_any_id));

  ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                      IP_EVENT_STA_GOT_IP,
                                                      wifi_controller_descriptor->event_handler,
                                                      NULL,
                                                      &instance_got_ip));

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_controller_descriptor->wifi_config) );
  ESP_ERROR_CHECK(esp_wifi_start() );

  ESP_LOGI(TAG, "wifi_init_sta finished.");

  /* 
   * Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
   * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) 
   */
  EventBits_t bits = xEventGroupWaitBits(event_group,
                                         WIFI_CONTROLLER_BIT_CONNECTED | WIFI_CONTROLLER_BIT_FAIL,
                                         pdFALSE,
                                         pdFALSE,
                                         portMAX_DELAY);

  /* 
   * xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
   * happened. 
   */
  if (bits & WIFI_CONTROLLER_BIT_CONNECTED) {
    ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
             WIFI_CONTROLLER_SETTINGS_SSID, WIFI_CONTROLLER_SETTINGS_PASSWORD);
  } else if (bits & WIFI_CONTROLLER_BIT_FAIL) {
    ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
             WIFI_CONTROLLER_SETTINGS_SSID, WIFI_CONTROLLER_SETTINGS_PASSWORD);
  } else {
    ESP_LOGE(TAG, "UNEXPECTED EVENT");
  }

  return WIFI_CONTROLLER_RESULT_SUCCESS;
}
