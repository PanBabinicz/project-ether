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

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#define MQTT_CONTROLLER_BROKER_ADDRESS_URI ("mqtt://192.168.72.80:1883")

typedef void (*mqtt_controller_event_handler_t)(void *, esp_event_base_t, int32_t, void *);

typedef enum {
  MQTT_CONTROLLER_RESULT_SUCCESS = 0,
  MQTT_CONTROLLER_RESULT_ERROR,
} mqtt_controller_result_t;

typedef struct {
  const esp_mqtt_client_config_t client_config;
  const mqtt_controller_event_handler_t event_handler;
} mqtt_controller_descriptor_t;

#define MQTT_CONTROLLER_CONFIG_DEFAULT  { \
  .broker.address.uri = MQTT_CONTROLLER_BROKER_ADDRESS_URI, \
}

#define MQTT_CONTROLLER_DESCRIPTOR_DEFAULT  { \
  .client_config = MQTT_CONTROLLER_CONFIG_DEFAULT, \
  .event_handler = mqtt_controller_event_handler, \
}

extern const mqtt_controller_descriptor_t mqtt_controller_descriptor_default;

void mqtt_controller_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

mqtt_controller_result_t mqtt_controller_init(const mqtt_controller_descriptor_t *mqtt_controller_descriptor);

#endif // !INC_MQTT_CONTROLLER_H
