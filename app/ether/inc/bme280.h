#ifndef INC_BME280_H
#define INC_BME280_H

#include <stddef.h>
#include <stdint.h>
#include "hal/i2c_types.h"
#include "i2c_controller.h"

#define BME280_I2C_ADDRESS  (0x76)

#define BME280_REGISTER_ID        	(0xd0)
#define BME280_REGISTER_RESET     	(0xe0)
#define BME280_REGISTER_CTRL_HUM  	(0xf2)
#define BME280_REGISTER_STATUS    	(0xf3) 
#define BME280_REGISTER_CTRL_MEAS 	(0xf4) 
#define BME280_REGISTER_CONFIG    	(0xf5) 
#define BME280_REGISTER_PRESS_MSB 	(0xf7)
#define BME280_REGISTER_PRESS_LSB 	(0xf8) 
#define BME280_REGISTER_PRESS_XLSB  (0xf9)
#define BME280_REGISTER_TEMP_MSB    (0xfa)
#define BME280_REGISTER_TEMP_LSB    (0xfb)
#define BME280_REGISTER_TEMP_XLSB   (0xfc)
#define BME280_REGISTER_HUM_MSB     (0xfd)
#define BME280_REGISTER_HUM_LSB     (0xfe)

#define BME280_REGISTER_CALIB00   (0x88)
#define BME280_REGISTER_CALIB01   (0x89)
#define BME280_REGISTER_CALIB02	  (0x8a)
#define BME280_REGISTER_CALIB03	  (0x8b)
#define BME280_REGISTER_CALIB04	  (0x8c)
#define BME280_REGISTER_CALIB05	  (0x8d)
#define BME280_REGISTER_CALIB06	  (0x8e)
#define BME280_REGISTER_CALIB07	  (0x8f)
#define BME280_REGISTER_CALIB08	  (0x90)
#define BME280_REGISTER_CALIB09	  (0x91)
#define BME280_REGISTER_CALIB10		(0x92)
#define BME280_REGISTER_CALIB11		(0x93)
#define BME280_REGISTER_CALIB12		(0x94)
#define BME280_REGISTER_CALIB13		(0x95)
#define BME280_REGISTER_CALIB14		(0x96)
#define BME280_REGISTER_CALIB15		(0x97)
#define BME280_REGISTER_CALIB16		(0x98)
#define BME280_REGISTER_CALIB17		(0x99)
#define BME280_REGISTER_CALIB18		(0x9a)
#define BME280_REGISTER_CALIB19		(0x9b)
#define BME280_REGISTER_CALIB20		(0x9c)
#define BME280_REGISTER_CALIB21		(0x9d)
#define BME280_REGISTER_CALIB22		(0x9e)
#define BME280_REGISTER_CALIB23		(0x9f)
#define BME280_REGISTER_CALIB24		(0xa0)
#define BME280_REGISTER_CALIB25		(0xa1)
#define BME280_REGISTER_CALIB26		(0xe1)
#define BME280_REGISTER_CALIB27		(0xe2)
#define BME280_REGISTER_CALIB28		(0xe3)
#define BME280_REGISTER_CALIB29		(0xe4)
#define BME280_REGISTER_CALIB30		(0xe5)
#define BME280_REGISTER_CALIB31		(0xe6)
#define BME280_REGISTER_CALIB32		(0xe7)
#define BME280_REGISTER_CALIB33		(0xe8)
#define BME280_REGISTER_CALIB34		(0xe9)
#define BME280_REGISTER_CALIB35		(0xea)
#define BME280_REGISTER_CALIB36		(0xeb)
#define BME280_REGISTER_CALIB37		(0xec)
#define BME280_REGISTER_CALIB38		(0xed)
#define BME280_REGISTER_CALIB39		(0xee)
#define BME280_REGISTER_CALIB40		(0xef)
#define BME280_REGISTER_CALIB41		(0xf0)

#define BME280_DATA_RESET (0xb6)
#define BME280_DATA_ID    (0x60)

#define BME280_SETTINGS_OSRS_H_SKIPPED  (0 << 0)
#define BME280_SETTINGS_OSRS_H_1        (1 << 0)
#define BME280_SETTINGS_OSRS_H_2        (2 << 0)
#define BME280_SETTINGS_OSRS_H_4        (3 << 0)
#define BME280_SETTINGS_OSRS_H_8        (4 << 0)
#define BME280_SETTINGS_OSRS_H_16       (5 << 0)

#define BME280_SETTINGS_OSRS_T_SKIPPED  (0 << 5)
#define BME280_SETTINGS_OSRS_T_1        (1 << 5)
#define BME280_SETTINGS_OSRS_T_2        (2 << 5)
#define BME280_SETTINGS_OSRS_T_4        (3 << 5)
#define BME280_SETTINGS_OSRS_T_8        (4 << 5)
#define BME280_SETTINGS_OSRS_T_16       (5 << 5)

#define BME280_SETTINGS_OSRS_P_SKIPPED  (0 << 2)
#define BME280_SETTINGS_OSRS_P_1        (1 << 2)
#define BME280_SETTINGS_OSRS_P_2        (2 << 2)
#define BME280_SETTINGS_OSRS_P_4        (3 << 2)
#define BME280_SETTINGS_OSRS_P_8        (4 << 2)
#define BME280_SETTINGS_OSRS_P_16       (5 << 2)

#define BME280_SETTINGS_MODE_SLEEP      (0 << 0)
#define BME280_SETTINGS_MODE_FORCE      (1 << 0)
#define BME280_SETTINGS_MODE_NORMAL     (3 << 0)

#define BME280_I2C_ACK_ENABLE   (0x01)
#define BME280_I2C_ACK_DISABLE  (0x00)

#define BME280_SIZE_HUM   (0x02)
#define BME280_SIZE_PRESS (0x03)
#define BME280_SIZE_TEMP  (0x03)
#define BME280_SIZE_COMP  (0x21)

/** 
 * \brief Result codes for BME280 sensor operations.
 */
typedef enum {
  BME280_RESULT_SUCCESS = 0,  /*!< Operation was successful. */
  BME280_RESULT_ERROR,        /*!< Operation encountered an error. */
} bme280_result_t;

/** 
 * \brief State codes for BME280 sensor initialization and operations.
 */
typedef enum {
  BME280_STATE_INIT = 0,                /*!< Initial state. */
  BME280_STATE_RESET,                   /*!< Reset state. */
  BME280_STATE_ID,                      /*!< ID reading state. */
  BME280_STATE_FORCE_MODE,              /*!< Force mode state. */
  BME280_STATE_MEASURE_HUMIDITY,        /*!< Humidity measurement state. */
  BME280_STATE_MEASURE_TEMPERATURE,     /*!< Temperature measurement state. */
  BME280_STATE_MEASURE_PRESSURE,        /*!< Pressure measurement state. */
  BME280_STATE_GET_COMPENSATION_DATA,   /*!< Compensation data retrieval state. */
  BME280_STATE_COMPENSATE_HUMIDITY,     /*!< Humidity compensation state. */
  BME280_STATE_COMPENSATE_TEMPERATURE,  /*!< Temperature compensation state. */
  BME280_STATE_COMPENSATE_PRESSURE,     /*!< Pressure compensation state. */
  BME280_STATE_UNSET = 0xFF,            /*!< Unset state. */
} bme280_state_t;

/** 
 * \brief Structure for the BME280 sensor settings.
 */
typedef struct {
  uint8_t ctrl_hum;   /*!< Control humidity register value. */
  uint8_t ctrl_meas;  /*!< Control measurement register value. */
  uint8_t config;     /*!< Configuration register value. */
} bme280_settings_t;

/** 
 * \brief Structure for the BME280 sensor pressure data.
 */
typedef struct {
  uint8_t msb;          /*!< Most significant byte. */
  uint8_t lsb;          /*!< Least significant byte. */
  uint8_t xlsb;         /*!< Extended least significant byte. */
  double  compensated;  /*!< Compensated pressure value. */
} bme280_pressure_t;

/** 
 * \brief Structure for the BME280 sensor temperature data.
 */
typedef struct {
  uint8_t msb;          /*!< Most significant byte. */
  uint8_t lsb;          /*!< Least significant byte. */
  uint8_t xlsb;         /*!< Extended least significant byte. */
  double  compensated;  /*!< Compensated temperature value. */
} bme280_temperature_t;

/** 
 * \brief Structure for the BME280 sensor humidity data.
 */
typedef struct {
  uint8_t msb;          /*!< Most significant byte. */
  uint8_t lsb;          /*!< Least significant byte. */
  double  compensated;  /*!< Compensated humidity value. */
} bme280_humidity_t;

/** 
 * \brief Structure for the BME280 sensor compensator data.
 */
typedef struct {
  uint16_t  dig_t1;     /*!< Temperature compensation value T1. */
  int16_t   dig_t2;     /*!< Temperature compensation value T2. */
  int16_t   dig_t3;     /*!< Temperature compensation value T3. */
  uint16_t  dig_p1;     /*!< Pressure compensation value P1. */
  int16_t   dig_p2;     /*!< Pressure compensation value P2. */
  int16_t   dig_p3;     /*!< Pressure compensation value P3. */
  int16_t   dig_p4;     /*!< Pressure compensation value P4. */
  int16_t   dig_p5;     /*!< Pressure compensation value P5. */
  int16_t   dig_p6;     /*!< Pressure compensation value P6. */
  int16_t   dig_p7;     /*!< Pressure compensation value P7. */
  int16_t   dig_p8;     /*!< Pressure compensation value P8. */
  int16_t   dig_p9;     /*!< Pressure compensation value P9. */
  uint8_t   dig_h1;     /*!< Humidity compensation value H1. */
  int16_t   dig_h2;     /*!< Humidity compensation value H2. */
  uint8_t   dig_h3;     /*!< Humidity compensation value H3. */
  int16_t   dig_h4;     /*!< Humidity compensation value H4. */
  int16_t   dig_h5;     /*!< Humidity compensation value H5. */
  int8_t    dig_h6;     /*!< Humidity compensation value H6. */
} bme280_compensator_t;

/** 
 * \brief Structure for BME280 sensor measurements.
 */
typedef struct {
  bme280_humidity_t humidity;         /*!< Humidity data. */
  bme280_pressure_t pressure;         /*!< Pressure data. */
  bme280_temperature_t temperature;   /*!< Temperature data. */
  bme280_compensator_t compensator;   /*!< Compensator data. */
} bme280_measurements_t;

/**
 * Suggested settings for weather monitoring;
 * Sensor mode: force mode, 1 sample per minute;
 */
#define BME280_SETTINGS_DEFAULT {                                     \
  .ctrl_hum = BME280_SETTINGS_OSRS_H_1,                               \
  .ctrl_meas = (BME280_SETTINGS_OSRS_T_1 | BME280_SETTINGS_OSRS_P_1 | \
                BME280_SETTINGS_MODE_FORCE),                          \
  .config = 0x00,                                                     \
}

/** 
 * \brief Initialize the BME280 sensor.
 * 
 * \param[in]   i2c_num: I2C port number.
 * \param[in]   settings: Pointer to sensor settings structure.
 * \return      Result of the initialization.
 */
bme280_result_t bme280_init(i2c_port_t i2c_num, const bme280_settings_t *settings);

/** 
 * \brief Reset the BME280 sensor.
 * 
 * \param[in]   i2c_num: I2C port number.
 * \return      Result of the reset operation.
 */
bme280_result_t bme280_reset(i2c_port_t i2c_num);

/** 
 * \brief Read the sensor ID.
 * 
 * \param[in]   i2c_num: I2C port number.
 * \param[out]  data: Pointer to buffer to store ID data.
 * \param[in]   data_len: Length of the buffer.
 * \return      Result of the ID read operation.
 */
bme280_result_t bme280_id(i2c_port_t i2c_num, uint8_t *data, size_t data_len);

/** 
 * \brief Set the sensor to force mode.
 * 
 * \param[in]   i2c_num: I2C port number.
 * \param[in]   settings: Pointer to sensor settings structure.
 * \return      Result of the operation.
 */
bme280_result_t bme280_force_mode(i2c_port_t i2c_num, const bme280_settings_t *settings);

/** 
 * \brief Measure humidity.
 * 
 * \param[in]   i2c_num: I2C port number.
 * \param[out]  humidity: Pointer to humidity structure to store the measurement.
 * \return      Result of the humidity measurement.
 */
bme280_result_t bme280_measure_humidity(i2c_port_t i2c_num, bme280_humidity_t *humidity);

/** 
 * \brief Measure temperature.
 * 
 * \param[in]   i2c_num: I2C port number.
 * \param[out]  temperature: Pointer to temperature structure to store the measurement.
 * \return      Result of the temperature measurement.
 */
bme280_result_t bme280_measure_temperature(i2c_port_t i2c_num, 
                                           bme280_temperature_t *temperature);

/** 
 * \brief Measure pressure.
 * 
 * \param[in]   i2c_num: I2C port number.
 * \param[out]  pressure: Pointer to pressure structure to store the measurement.
 * \return      Result of the pressure measurement.
 */
bme280_result_t bme280_measure_pressure(i2c_port_t i2c_num, bme280_pressure_t *pressure);

/** 
 * \brief Get sensor compensation data.
 * 
 * \param[in]   i2c_num: I2C port number.
 * \param[out]  compensator: Pointer to compensator structure to store the data.
 * \return      Result of the compensation data retrieval.
 */
bme280_result_t bme280_get_compensation_data(i2c_port_t i2c_num, 
                                             bme280_compensator_t *compensator);

/** 
 * \brief Compensate the humidity measurement.
 * 
 * \param[in]   compensator: Pointer to compensator structure with calibration data.
 * \param[out]  humidity: Pointer to humidity structure to store compensated value.
 * \return      Result of the humidity compensation.
 */
bme280_result_t bme280_compensate_humidity(bme280_compensator_t *compensator, 
                                           bme280_humidity_t *humidity);

/** 
 * \brief Compensate the temperature measurement.
 * 
 * \param[in]   compensator: Pointer to compensator structure with calibration data.
 * \param[out]  temperature: Pointer to temperature structure to store compensated value.
 * \return      Result of the temperature compensation.
 */
bme280_result_t bme280_compensate_temperature(bme280_compensator_t *compensator, 
                                              bme280_temperature_t *temperature);

/** 
 * \brief Compensate the pressure measurement.
 * 
 * \param[in]   compensator: Pointer to compensator structure with calibration data.
 * \param[out]  pressure: Pointer to pressure structure to store compensated value.
 * \return      Result of the pressure compensation.
 */
bme280_result_t bme280_compensate_pressure(bme280_compensator_t *compensator, 
                                           bme280_pressure_t *pressure);

#endif // !INC_BME280_H
