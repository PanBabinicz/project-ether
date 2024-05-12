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
 * \ param
 * \ param
 */
typedef enum {
  BME280_RESULT_SUCCESS = 0,
  BME280_RESULT_ERROR,
} bme280_result_t;

/** 
 * \ param
 * \ param
 */
typedef enum {
  BME280_STATUS_MEASURING = 0,
  BME280_STATUS_IM_UPDATE,
} bme280_status_t;

/** 
 * \ param
 * \ param
 */
typedef struct {
  uint8_t ctrl_hum;
  uint8_t ctrl_meas;
  uint8_t config;
} bme280_settings_t;

/** 
 * \ param
 * \ param
 */
typedef struct {
  uint8_t msb;
  uint8_t lsb;
  uint8_t xlsb;
  double  compensated;
} bme280_pressure_t;

/** 
 * \ param
 * \ param
 */
typedef struct {
  uint8_t msb;
  uint8_t lsb;
  uint8_t xlsb;
  double  compensated;
} bme280_temperature_t;

/** 
 * \ param
 * \ param
 */
typedef struct {
  uint8_t msb;
  uint8_t lsb;
  double  compensated;
} bme280_humidity_t;

/** 
 * \ param
 * \ param
 */
typedef struct {
  uint16_t  dig_t1;
  int16_t   dig_t2;
  int16_t   dig_t3;
  uint16_t  dig_p1;
  int16_t   dig_p2;
  int16_t   dig_p3;
  int16_t   dig_p4;
  int16_t   dig_p5;
  int16_t   dig_p6;
  int16_t   dig_p7;
  int16_t   dig_p8;
  int16_t   dig_p9;
  uint8_t   dig_h1;
  int16_t   dig_h2;
  uint8_t   dig_h3;
  int16_t   dig_h4;
  int16_t   dig_h5;
  int8_t    dig_h6;
} bme280_compensator_t;

/** 
 * \ param
 * \ param
 */
typedef struct {
  bme280_humidity_t humidity;
  bme280_pressure_t pressure;
  bme280_temperature_t temperature;
  bme280_compensator_t compensator;
} bme280_measurements_t;

/**
 * Suggested settings for weather monitoring;
 * Sensor mode: force mode, 1 sample per minute;
 */
#define BME280_DEFAULT_SETTINGS {                                     \
  .ctrl_hum = BME280_SETTINGS_OSRS_H_1,                               \
  .ctrl_meas = (BME280_SETTINGS_OSRS_T_1 | BME280_SETTINGS_OSRS_P_1 | \
                BME280_SETTINGS_MODE_FORCE),                          \
  .config = 0x00,                                                     \
}

/** 
 * \ param
 * \ param
 */
extern const bme280_settings_t bme280_default_settings;

/** 
 * \ param
 * \ param
 */
bme280_result_t bme280_init(i2c_port_t i2c_num, const bme280_settings_t *settings);

/** 
 * \ param
 * \ param
 */
bme280_result_t bme280_reset(i2c_port_t i2c_num);

/** 
 * \ param
 * \ param
 */
bme280_result_t bme280_id(i2c_port_t i2c_num, uint8_t *data, size_t data_len);

/** 
 * \ param
 * \ param
 */
bme280_result_t bme280_force_mode(i2c_port_t i2c_num, const bme280_settings_t *settings);

/** 
 * \ param
 * \ param
 */
bme280_result_t bme280_measure_humidity(i2c_port_t i2c_num, bme280_humidity_t *humidity);

/** 
 * \ param
 * \ param
 */
bme280_result_t bme280_measure_temperature(i2c_port_t i2c_num, 
                                           bme280_temperature_t *temperature);

/** 
 * \ param
 * \ param
 */
bme280_result_t bme280_measure_pressure(i2c_port_t i2c_num, bme280_pressure_t *pressure);

/** 
 * \ param
 * \ param
 */
bme280_result_t bme280_get_compensation_data(i2c_port_t i2c_num, 
                                             bme280_compensator_t *compensator);

/** 
 * \ param
 * \ param
 */
bme280_result_t bme280_compensate_humidity(bme280_compensator_t *compensator, 
                                           bme280_humidity_t *humidity);

/** 
 * \ param
 * \ param
 */
bme280_result_t bme280_compensate_temperature(bme280_compensator_t *compensator, 
                                              bme280_temperature_t *temperature);

/** 
 * \ param
 * \ param
 */
bme280_result_t bme280_compensate_pressure(bme280_compensator_t *compensator, 
                                           bme280_pressure_t *pressure);

/** 
 * \ param
 * \ param
 */
bme280_status_t bme280_status(i2c_port_t i2c_num);

#endif // !INC_BME280_H
