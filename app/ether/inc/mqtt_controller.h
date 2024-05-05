#ifndef INC_MQTT_CONTROLLER_H
#define INC_MQTT_CONTROLLER_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

typedef enum {
  MQTT_CONTROLLER_RESULT_SUCCESS = 0,
  MQTT_CONTROLLER_RESULT_ERROR,
} mqtt_controller_result_t;

typedef struct {
  const esp_mqtt_client_config_t client_config;
  esp_mqtt_client_handle_t client_handle;
} mqtt_controller_descriptor_t;

#define MQTT_CONTROLLER_CONFIG_DEFAULT  { \
  .broker.
}

#define MQTT_CONTROLLER_DESCRIPTOR_DEFAULT  { \
  .esp_mqtt_client_config = MQTT_CONTROLLER_CONFIG_DEFAULT, \
}

mqtt_controller_result_t mqtt_controller_init(mqtt_controller_descriptor_t *mqtt_controller_descriptor);

#endif // !INC_MQTT_CONTROLLER_H
