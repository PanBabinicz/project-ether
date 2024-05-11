idf_component_register(
  SRCS 
    "ether_main.c" 
    "../src/pms7003.c" 
    "../src/bme280.c" 
    "../src/i2c_controller.c" 
    "../src/uart_controller.c" 
    "../src/wifi_controller.c"
    "../src/mqtt_controller.c"
  INCLUDE_DIRS 
    "." 
    "../inc"
)

if (NOT DEFINED ENV{ESP32_WIFI_SSID})
  MESSAGE (FATAL_ERROR "ESP32_WIFI_SSID is not set as environment variable...")
endif()

if (NOT DEFINED ENV{ESP32_WIFI_PASSWORD})
  MESSAGE (FATAL_ERROR "ESP32_WIFI_PASSWORD is not set as environment variable...")
endif()

# Get the value of the environment variable
set(ESP32_WIFI_PASSWORD "\"$ENV{ESP32_WIFI_PASSWORD}\"")

# Add a definition for the preprocessor macro
add_definitions(-DWIFI_CONTROLLER_SETTINGS_PASSWORD=${ESP32_WIFI_PASSWORD})

set(ESP32_WIFI_SSID "\"$ENV{ESP32_WIFI_SSID}\"")
add_definitions(-DWIFI_CONTROLLER_SETTINGS_SSID=${ESP32_WIFI_SSID})
