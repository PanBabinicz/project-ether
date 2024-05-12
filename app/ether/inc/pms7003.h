#ifndef INC_PMS7003_H
#define INC_PMS7003_H

#include <stdint.h>
#include "driver/uart.h"

#define PMS7003_START_CHARACTER_1 (0x42)
#define PMS7003_START_CHARACTER_2 (0x4d)
#define PMS7003_CMD_READ          (0xe2)
#define PMS7003_CMD_CHANGE_MODE   (0xe1)
#define PMS7003_CMD_SLEEP_SET     (0xe4)

#define PMS7003_FRAME_REQUEST_SIZE    (0x07)
#define PMS7003_FRAME_ANSWER_SIZE     (0x20)
#define PMS7003_FRAME_CHECK_CODE_SIZE (0x1e)
#define PMS7003_FRAME_BYTE_SIZE       (0x01)

#define PMS7003_UART_WAIT_TIMEOUT_MS  (0x64)

/** 
 * \ param
 * \ param
 */
typedef union {
  struct {
    const uint8_t start_byte_1;
    const uint8_t start_byte_2;
    uint8_t command;
    uint8_t data_h;
    uint8_t data_l;
    /* Verify bytes: add all the bytes except verify bytes. */
    uint8_t lrch;     
    uint8_t lrcl;
  } __attribute__((packed));  // Ensure no padding between members
  uint8_t buffer_request[PMS7003_FRAME_REQUEST_SIZE];
} pms7003_frame_request_t;

/** 
 * \ param
 * \ param
 */
typedef union {
  struct {
    uint8_t start_byte_1;
    uint8_t start_byte_2;
    uint16_t length;
    uint16_t data_pm1_standard;
    uint16_t data_pm25_standard;
    uint16_t data_pm10_standard;
    uint16_t data_pm1_atmospheric;
    uint16_t data_pm25_atmospheric;
    uint16_t data_concentration_unit;
    uint16_t data_particles_300nm;
    uint16_t data_particles_500nm;
    uint16_t data_particles_1000nm;
    uint16_t data_particles_2500nm;
    uint16_t data_particles_5000nm;
    uint16_t data_particles_10000nm;
    uint16_t reserved;
    uint16_t check_code;
  } __attribute__((packed));  // Ensure no padding between members
  uint8_t buffer_answer[PMS7003_FRAME_ANSWER_SIZE];
} pms7003_frame_answer_t;

/** 
 * \ param
 * \ param
 */
typedef enum {
  PMS7003_RESULT_SUCCESS = 0,
  PMS7003_RESULT_ERROR,
  PMS7003_RESULT_PARTIAL_SENT,
  PMS7003_RESULT_PARTIAL_RECEIVED,
  PMS7003_RESULT_WRONG_CHECK_CODE,
} pms7003_result_t;

/** 
 * \ param
 * \ param
 */
typedef enum {
  PMS7003_STATUS_OK = 0,
  PMS7003_STATUS_READY,
} pms7003_status_t;

/** 
 * \ param
 * \ param
 */
#define PMS7003_FRAME_READ { \
  .start_byte_1 = PMS7003_START_CHARACTER_1, \
  .start_byte_2 = PMS7003_START_CHARACTER_2, \
  .command = PMS7003_CMD_READ, \
  .data_h = 0x00, \
  .data_l = 0x00, \
  .lrch = 0x01, \
  .lrcl = 0x71 \
} 

/** 
 * \ param
 * \ param
 */
#define PMS7003_FRAME_CHANGE_MODE_PASSIVE { \
  .start_byte_1 = PMS7003_START_CHARACTER_1, \
  .start_byte_2 = PMS7003_START_CHARACTER_2, \
  .command = PMS7003_CMD_CHANGE_MODE, \
  .data_h = 0x00, \
  .data_l = 0x00, \
  .lrch = 0x01, \
  .lrcl = 0x70 \
} 

/** 
 * \ param
 * \ param
 */
#define PMS7003_FRAME_CHANGE_MODE_ACTIVE { \
  .start_byte_1 = PMS7003_START_CHARACTER_1, \
  .start_byte_2 = PMS7003_START_CHARACTER_2, \
  .command = PMS7003_CMD_CHANGE_MODE, \
  .data_h = 0x00, \
  .data_l = 0x01, \
  .lrch = 0x01, \
  .lrcl = 0x71 \
} 

/** 
 * \ param
 * \ param
 */
#define PMS7003_FRAME_SLEEP { \
  .start_byte_1 = PMS7003_START_CHARACTER_1, \
  .start_byte_2 = PMS7003_START_CHARACTER_2, \
  .command = PMS7003_CMD_SLEEP_SET, \
  .data_h = 0x00, \
  .data_l = 0x00, \
  .lrch = 0x01, \
  .lrcl = 0x73 \
} 

/** 
 * \ param
 * \ param
 */
#define PMS7003_FRAME_WAKEUP { \
  .start_byte_1 = PMS7003_START_CHARACTER_1, \
  .start_byte_2 = PMS7003_START_CHARACTER_2, \
  .command = PMS7003_CMD_SLEEP_SET, \
  .data_h = 0x00, \
  .data_l = 0x01, \
  .lrch = 0x01, \
  .lrcl = 0x74 \
} 

/** 
 * \ param
 * \ param
 */
typedef int32_t (*pms7003_callback_sent_t)(uart_port_t);

/** 
 * \ param
 * \ param
 */
typedef int32_t (*pms7003_callback_received_t)(uart_port_t, pms7003_frame_answer_t *);

/** 
 * \ param
 * \ param
 */
pms7003_result_t pms7003_frame_send(const pms7003_callback_sent_t handler, uart_port_t uart_num);

/** 
 * \ param
 * \ param
 */
pms7003_result_t pms7003_frame_receive(const pms7003_callback_received_t handler, 
                                       uart_port_t uart_num, pms7003_frame_answer_t *frame);

/** 
 * \ param
 * \ param
 */
int32_t pms7003_read_request(uart_port_t uart_num);

/** 
 * \ param
 * \ param
 */
int32_t pms7003_change_mode_passive(uart_port_t uart_num);

/** 
 * \ param
 * \ param
 */
int32_t pms7003_change_mode_active(uart_port_t uart_num);

/** 
 * \ param
 * \ param
 */
int32_t pms7003_sleep(uart_port_t uart_num);

/** 
 * \ param
 * \ param
 */
int32_t pms7003_wakeup(uart_port_t uart_num);

/** 
 * \ param
 * \ param
 */
int32_t pms7003_read(uart_port_t uart_num, pms7003_frame_answer_t *frame);

#endif // !INC_PMS7003_H
