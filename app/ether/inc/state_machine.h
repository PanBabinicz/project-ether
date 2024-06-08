#ifndef INC_STATE_MACHINE_H
#define INC_STATE_MACHINE_H

#include "ether.h"

/** 
 * \brief Initialize the BME280 sensor within the state machine.
 * 
 * \param[out]  ether: Pointer to the ether structure.
 * \return      Result of the initialization operation.
 */
bme280_result_t state_machine_bme280_init(ether_t *ether);

/** 
 * \brief Reset the BME280 sensor within the state machine.
 * 
 * \param[out]  ether: Pointer to the ether structure.
 * \return      Result of the reset operation.
 */
bme280_result_t state_machine_bme280_reset(ether_t *ether);

/** 
 * \brief Read the ID of the BME280 sensor within the state machine.
 * 
 * \param[out]  ether: Pointer to the ether structure.
 * \return      Result of the ID read operation.
 */
bme280_result_t state_machine_bme280_id(ether_t *ether);

/** 
 * \brief Set the BME280 sensor to force mode within the state machine.
 * 
 * \param[out]  ether: Pointer to the ether structure.
 * \return      Result of the force mode operation.
 */
bme280_result_t state_machine_bme280_force_mode(ether_t *ether);

/** 
 * \brief Measure humidity with the BME280 sensor within the state machine.
 * 
 * \param[out]  ether: Pointer to the ether structure.
 * \return      Result of the humidity measurement operation.
 */
bme280_result_t state_machine_bme280_measure_humidity(ether_t *ether);

/** 
 * \brief Measure temperature with the BME280 sensor within the state machine.
 * 
 * \param[out]  ether: Pointer to the ether structure.
 * \return      Result of the temperature measurement operation.
 */
bme280_result_t state_machine_bme280_measure_temperature(ether_t *ether);

/** 
 * \brief Measure pressure with the BME280 sensor within the state machine.
 * 
 * \param[out]  ether: Pointer to the ether structure.
 * \return      Result of the pressure measurement operation.
 */
bme280_result_t state_machine_bme280_measure_pressure(ether_t *ether);

/** 
 * \brief Get compensation data from the BME280 sensor within the state machine.
 * 
 * \param[out]  ether: Pointer to the ether structure.
 * \return      Result of the get compensation data operation.
 */
bme280_result_t state_machine_bme280_get_compensation_data(ether_t *ether);

/** 
 * \brief Compensate humidity measurement from the BME280 sensor within the state machine.
 * 
 * \param[out]  ether: Pointer to the ether structure.
 * \return      Result of the humidity compensation operation.
 */
bme280_result_t state_machine_bme280_compensate_humidity(ether_t *ether);

/** 
 * \brief Compensate temperature measurement from the BME280 sensor within the state machine.
 * 
 * \param[out]  ether: Pointer to the ether structure.
 * \return      Result of the temperature compensation operation.
 */
bme280_result_t state_machine_bme280_compensate_temperature(ether_t *ether);

/** 
 * \brief Compensate pressure measurement from the BME280 sensor within the state machine.
 * 
 * \param[out]  ether: Pointer to the ether structure.
 * \return      Result of the pressure compensation operation.
 */
bme280_result_t state_machine_bme280_compensate_pressure(ether_t *ether);

#endif // !INC_STATE_MACHINE_H
