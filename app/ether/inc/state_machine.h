#ifndef INC_STATE_MACHINE_H
#define INC_STATE_MACHINE_H

#include "ether.h"

/** 
 * \param
 * \param
 */
bme280_result_t state_machine_bme280_init(ether_t *ether);

/** 
 * \param
 * \param
 */
bme280_result_t state_machine_bme280_reset(ether_t *ether);

/** 
 * \param
 * \param
 */
bme280_result_t state_machine_bme280_id(ether_t *ether);

/** 
 * \param
 * \param
 */
bme280_result_t state_machine_bme280_force_mode(ether_t *ether);

/** 
 * \param
 * \param
 */
bme280_result_t state_machine_bme280_measure_humidity(ether_t *ether);

/** 
 * \param
 * \param
 */
bme280_result_t state_machine_bme280_measure_temperature(ether_t *ether);

/** 
 * \param
 * \param
 */
bme280_result_t state_machine_bme280_measure_pressure(ether_t *ether);

/** 
 * \param
 * \param
 */
bme280_result_t state_machine_bme280_get_compensation_data(ether_t *ether);

/** 
 * \param
 * \param
 */
bme280_result_t state_machine_bme280_compensate_humidity(ether_t *ether);

/** 
 * \param
 * \param
 */
bme280_result_t state_machine_bme280_compensate_temperature(ether_t *ether);

/** 
 * \param
 * \param
 */
bme280_result_t state_machine_bme280_compensate_pressure(ether_t *ether);

#endif // !INC_STATE_MACHINE_H
