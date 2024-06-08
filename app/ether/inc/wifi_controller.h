#ifndef INC_WIFI_CONTROLLER_H
#define INC_WIFI_CONTROLLER_H

#include <stdint.h>

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi_types.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define WIFI_CONTROLLER_BIT_CONNECTED (BIT0)
#define WIFI_CONTROLLER_BIT_FAIL      (BIT1)

#define WIFI_CONTROLLER_RETRY_MAX (0x05)

/** 
 * \brief Result codes for WIFI controller operations.
 */
typedef enum {
  WIFI_CONTROLLER_RESULT_SUCCESS = 0,   /*!< Operation was successful. */
  WIFI_CONTROLLER_RESULT_ERROR,         /*!< Operation encountered an error. */
} wifi_controller_result_t;

/** 
 * \brief Function pointer type for WIFI event handler.
 * 
 * \param[in]   arg: Pointer to user-defined data.
 * \param[in]   event_base: Base ID of the event.
 * \param[in]   event_id: ID of the event.
 * \param[in]   event_data: Pointer to event-specific data.
 */
typedef void (*wifi_controller_event_handler_t)(void *, esp_event_base_t, int32_t, void *);

/** 
 * \brief Structure for WIFI controller descriptor.
 */
typedef struct {
  wifi_config_t wifi_config;                        /*!< WIFI configuration. */
  wifi_controller_event_handler_t event_handler;    /*!< WIFI event handler. */
} wifi_controller_descriptor_t;

/**
 * \brief Default WiFi controller configuration and descriptor if SSID and password are defined.
 */
#if defined (WIFI_CONTROLLER_SETTINGS_SSID) && defined (WIFI_CONTROLLER_SETTINGS_PASSWORD)
  #define WIFI_CONTROLLER_CONFIG_DEFAULT  {           \
    .sta = {                                          \
      .ssid = WIFI_CONTROLLER_SETTINGS_SSID,          \
      .password = WIFI_CONTROLLER_SETTINGS_PASSWORD,  \
    },                                                \
  }

  #define WIFI_CONTROLLER_DESCRIPTOR_DEFAULT  {       \
    .wifi_config = WIFI_CONTROLLER_CONFIG_DEFAULT,    \
    .event_handler = wifi_controller_event_handler,   \
  }
#endif

/** 
 * \brief Event handler for WiFi events.
 * 
 * \param[in]   arg: Pointer to user-defined data.
 * \param[in]   event_base: Base ID of the event.
 * \param[in]   event_id: ID of the event.
 * \param[in]   event_data: Pointer to event-specific data.
 */
void wifi_controller_event_handler(void *arg, esp_event_base_t event_base, 
                                   int32_t event_id, void *event_data);

/** 
 * \brief Initialize the WiFi controller.
 * 
 * \param[in]   wifi_controller_descriptor: Pointer to the WiFi controller descriptor.
 * \return      Result of the initialization operation.
 */
wifi_controller_result_t wifi_controller_init(const wifi_controller_descriptor_t *wifi_controller_descriptor);

#endif // !INC_WIFI_CONTROLLER_H
