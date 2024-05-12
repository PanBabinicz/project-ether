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
 * \param
 * \param
 */
typedef enum {
  WIFI_CONTROLLER_RESULT_SUCCESS = 0,
  WIFI_CONTROLLER_RESULT_ERROR,
} wifi_controller_result_t;

/** 
 * \param
 * \param
 */
typedef void (*wifi_controller_event_handler_t)(void *, esp_event_base_t, int32_t, void *);

/** 
 * \param
 * \param
 */
typedef struct {
  const wifi_config_t wifi_config;
  const wifi_controller_event_handler_t event_handler;
} wifi_controller_descriptor_t;

/** 
 * \param
 * \param
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
 * \param
 * \param
 */
extern const wifi_controller_descriptor_t wifi_controller_descriptor_default;

/** 
 * \param
 * \param
 */
void wifi_controller_event_handler(void *arg, esp_event_base_t event_base, 
                                   int32_t event_id, void *event_data);

/** 
 * \param
 * \param
 */
wifi_controller_result_t wifi_controller_init(const wifi_controller_descriptor_t *wifi_controller_descriptor);

#endif // !INC_WIFI_CONTROLLER_H
