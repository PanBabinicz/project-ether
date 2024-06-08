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

#define MQTT_CONTROLLER_BROKER_ADDRESS_URI  ("mqtt://192.168.235.80:1883")
#define MQTT_CONTROLLER_MESSAGE_MAX_SIZE    (256)

/** 
 * \brief Function pointer type for MQTT event handler.
 * 
 * \param[in]   handler_args: Arguments passed to the handler.
 * \param[in]   base: Event base.
 * \param[in]   event_id: Event ID.
 * \param[in]   event_data: Event data.
 */
typedef void (*mqtt_controller_event_handler_t)(void *, esp_event_base_t, int32_t, void *);

/** 
 * \brief Result codes for MQTT controller operations.
 */
typedef enum {
  MQTT_CONTROLLER_RESULT_SUCCESS = 0,   /*!< Operation was successful. */
  MQTT_CONTROLLER_RESULT_ERROR,         /*!< Operation encountered an error. */
} mqtt_controller_result_t;

/** 
 * \brief Structure for MQTT controller descriptor.
 */
typedef struct {
  esp_mqtt_client_config_t client_config;           /*!< MQTT client configuration. */
  esp_mqtt_client_handle_t client_handle;           /*!< MQTT client handle. */
  mqtt_controller_event_handler_t event_handler;    /*!< MQTT event handler. */
} mqtt_controller_descriptor_t;

/** 
 * \brief Default configuration for the MQTT controller.
 */
#define MQTT_CONTROLLER_CONFIG_DEFAULT  {                   \
  .broker.address.uri = MQTT_CONTROLLER_BROKER_ADDRESS_URI, \
}

/** 
 * \brief Default descriptor for the MQTT controller.
 */
#define MQTT_CONTROLLER_DESCRIPTOR_DEFAULT  {       \
  .client_config = MQTT_CONTROLLER_CONFIG_DEFAULT,  \
  .event_handler = mqtt_controller_event_handler, \
}

/** 
 * \brief Handle MQTT events.
 * 
 * \param[in]   handler_args: Arguments passed to the handler.
 * \param[in]   base: Event base.
 * \param[in]   event_id: Event ID.
 * \param[in]   event_data: Event data.
 */
void mqtt_controller_event_handler(void *handler_args, esp_event_base_t base, 
                                   int32_t event_id, void *event_data);

/** 
 * \brief Initialize the MQTT controller.
 * 
 * \param[out]  mqtt_controller_descriptor: Pointer to the MQTT controller descriptor.
 * \return      Result of the initialization.
 */
mqtt_controller_result_t mqtt_controller_init(mqtt_controller_descriptor_t *mqtt_controller_descriptor);

#endif // !INC_MQTT_CONTROLLER_H
