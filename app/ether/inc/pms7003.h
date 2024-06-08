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
 * \brief Union representing a PMS7003 frame request.
 */
typedef union {
  struct {
    const uint8_t start_byte_1;                         /*!< Start byte 1. */
    const uint8_t start_byte_2;                         /*!< Start byte 2. */
    uint8_t command;                                    /*!< Command byte. */
    uint8_t data_h;                                     /*!< Data high byte. */
    uint8_t data_l;                                     /*!< Data low byte. */
    uint8_t lrch;                                       /*!< High byte of the checksum. */
    uint8_t lrcl;                                       /*!< Low byte of the checksum. */
    /* Verify bytes: add all the bytes except verify bytes. */
  } __attribute__((packed));  /* Ensure no padding between members. */
  uint8_t buffer_request[PMS7003_FRAME_REQUEST_SIZE];   /*!< Buffer for request frame. */
} pms7003_frame_request_t;

/** 
 * \brief Union representing a PMS7003 frame answer.
 */
typedef union {
  struct {
    uint8_t start_byte_1;                             /*!< Start byte 1. */
    uint8_t start_byte_2;                             /*!< Start byte 2. */
    uint16_t length;                                  /*!< Length of the frame. */
    uint16_t data_pm1_standard;                       /*!< PM1.0 concentration (standard particles). */
    uint16_t data_pm25_standard;                      /*!< PM2.5 concentration (standard particles). */
    uint16_t data_pm10_standard;                      /*!< PM10 concentration (standard particles). */
    uint16_t data_pm1_atmospheric;                    /*!< PM1.0 concentration (atmospheric particles). */
    uint16_t data_pm25_atmospheric;                   /*!< PM2.5 concentration (atmospheric particles). */
    uint16_t data_concentration_unit;                 /*!< Concentration unit. */
    uint16_t data_particles_300nm;                    /*!< Particles count for 0.3µm. */
    uint16_t data_particles_500nm;                    /*!< Particles count for 0.5µm. */
    uint16_t data_particles_1000nm;                   /*!< Particles count for 1.0µm. */
    uint16_t data_particles_2500nm;                   /*!< Particles count for 2.5µm. */
    uint16_t data_particles_5000nm;                   /*!< Particles count for 5.0µm. */
    uint16_t data_particles_10000nm;                  /*!< Particles count for 10µm. */
    uint16_t reserved;                                /*!< Reserved bytes. */
    uint16_t check_code;                              /*!< Checksum code. */
  } __attribute__((packed));  /* Ensure no padding between members. */
  uint8_t buffer_answer[PMS7003_FRAME_ANSWER_SIZE];   /*!< Buffer for answer frame. */
} pms7003_frame_answer_t;

/** 
 * \brief Structure representing PMS7003 measurements.
 */
typedef struct {
  uint16_t pm1;             /*!< PM1.0 measurement. */
  uint16_t pm25;            /*!< PM2.5 measurement. */
  uint16_t pm10;            /*!< PM10 measurement. */
} pms7003_measurements_t;

/** 
 * \brief Result codes for PMS7003 sensor operations.
 */
typedef enum {
  PMS7003_RESULT_SUCCESS = 0,         /*!< Operation was successful. */
  PMS7003_RESULT_ERROR,               /*!< Operation encountered an error. */
  PMS7003_RESULT_PARTIAL_SENT,        /*!< Partial data sent. */
  PMS7003_RESULT_PARTIAL_RECEIVED,    /*!< Partial data received. */
  PMS7003_RESULT_WRONG_CHECK_CODE,    /*!< Wrong checksum. */
} pms7003_result_t;

/** 
 * \brief Status codes for PMS7003 sensor.
 */
typedef enum {
  PMS7003_STATUS_OK = 0,    /*!< Status OK. */
  PMS7003_STATUS_READY,     /*!< Status Ready. */
} pms7003_status_t;

/** 
 * \brief States of the PMS7003 sensor.
 */
typedef enum {
  PMS7003_STATE_READ_REQUEST = 0,       /*!< Read request state. */
  PMS7003_STATE_CHANGE_MODE_PASSIVE,    /*!< Change to passive mode state. */
  PMS7003_STATE_CHANGE_MODE_ACTIVE,     /*!< Change to active mode state. */
  PMS7003_STATE_SLEEP,                  /*!< Sleep state. */
  PMS7003_STATE_WAKEUP,                 /*!< Wakeup state. */
  PMS7003_STATE_READ,                   /*!< Read state. */
  PMS7003_STATE_UNSET = 0xFF,           /*!< Unset state. */
} pms7003_state_t;

/** 
 * \brief Default frame for reading PMS7003 sensor data.
 */
#define PMS7003_FRAME_READ {                  \
  .start_byte_1 = PMS7003_START_CHARACTER_1,  \
  .start_byte_2 = PMS7003_START_CHARACTER_2,  \
  .command = PMS7003_CMD_READ,                \
  .data_h = 0x00,                             \
  .data_l = 0x00,                             \
  .lrch = 0x01,                               \
  .lrcl = 0x71,                               \
} 

/** 
 * \brief Default frame for changing PMS7003 sensor mode to passive.
 */
#define PMS7003_FRAME_CHANGE_MODE_PASSIVE {   \
  .start_byte_1 = PMS7003_START_CHARACTER_1,  \
  .start_byte_2 = PMS7003_START_CHARACTER_2,  \
  .command = PMS7003_CMD_CHANGE_MODE,         \
  .data_h = 0x00,                             \
  .data_l = 0x00,                             \
  .lrch = 0x01,                               \
  .lrcl = 0x70,                               \
} 

/** 
 * \brief Default frame for changing PMS7003 sensor mode to active.
 */
#define PMS7003_FRAME_CHANGE_MODE_ACTIVE {    \
  .start_byte_1 = PMS7003_START_CHARACTER_1,  \
  .start_byte_2 = PMS7003_START_CHARACTER_2,  \
  .command = PMS7003_CMD_CHANGE_MODE,         \
  .data_h = 0x00,                             \
  .data_l = 0x01,                             \
  .lrch = 0x01,                               \
  .lrcl = 0x71,                               \
} 

/** 
 * \brief Default frame for putting PMS7003 sensor to sleep.
 */
#define PMS7003_FRAME_SLEEP {                 \
  .start_byte_1 = PMS7003_START_CHARACTER_1,  \
  .start_byte_2 = PMS7003_START_CHARACTER_2,  \
  .command = PMS7003_CMD_SLEEP_SET,           \
  .data_h = 0x00,                             \
  .data_l = 0x00,                             \
  .lrch = 0x01,                               \
  .lrcl = 0x73,                               \
} 

/** 
 * \brief Default frame for waking PMS7003 sensor up.
 */
#define PMS7003_FRAME_WAKEUP {                \
  .start_byte_1 = PMS7003_START_CHARACTER_1,  \
  .start_byte_2 = PMS7003_START_CHARACTER_2,  \
  .command = PMS7003_CMD_SLEEP_SET,           \
  .data_h = 0x00,                             \
  .data_l = 0x01,                             \
  .lrch = 0x01,                               \
  .lrcl = 0x74,                               \
} 

/** 
 * \brief Callback function type for handling sent frames.
 * 
 * \param[in]   uart_num: UART port number.
 * \return      Status code of the operation.
 */
typedef int32_t (*pms7003_callback_sent_t)(uart_port_t);

/** 
 * \brief Callback function type for handling received frames.
 * 
 * \param[in]   uart_num: UART port number.
 * \param[out]  frame: Pointer to the frame answer structure.
 * \return      Status code of the operation.
 */
typedef int32_t (*pms7003_callback_received_t)(uart_port_t, pms7003_frame_answer_t *);

/** 
 * \brief Send a PMS7003 frame.
 * 
 * \param[in]   handler: Callback function to handle the sent frame.
 * \param[in]   uart_num: UART port number.
 * \return      Result of the send operation.
 */
pms7003_result_t pms7003_frame_send(const pms7003_callback_sent_t handler, uart_port_t uart_num);

/** 
 * \brief Receive a PMS7003 frame.
 * 
 * \param[in]   handler: Callback function to handle the received frame.
 * \param[in]   uart_num: UART port number.
 * \param[out]  frame: Pointer to the frame answer structure.
 * \return      Result of the receive operation.
 */
pms7003_result_t pms7003_frame_receive(const pms7003_callback_received_t handler, 
                                       uart_port_t uart_num, pms7003_frame_answer_t *frame);

/** 
 * \brief Send a read request to the PMS7003 sensor.
 * 
 * \param[in]   uart_num: UART port number.
 * \return      Status code of the operation.
 */
int32_t pms7003_read_request(uart_port_t uart_num);

/** 
 * \brief Change the PMS7003 mode to passive.
 * 
 * \param[in]   uart_num: UART port number.
 * \return      Status code of the operation.
 */
int32_t pms7003_change_mode_passive(uart_port_t uart_num);

/** 
 * \brief Change the PMS7003 mode to active.
 * 
 * \param[in]   uart_num: UART port number.
 * \return      Status code of the operation.
 */
int32_t pms7003_change_mode_active(uart_port_t uart_num);

/** 
 * \brief Put the PMS7003 sensor to sleep.
 * 
 * \param[in]   uart_num: UART port number.
 * \return      Status code of the operation.
 */
int32_t pms7003_sleep(uart_port_t uart_num);

/** 
 * \brief Wake the PMS7003 sensor up.
 * 
 * \param[in]   uart_num: UART port number.
 * \return      Status code of the operation.
 */
int32_t pms7003_wakeup(uart_port_t uart_num);

/** 
 * \brief Read data from the PMS7003 sensor.
 * 
 * \param[in]   uart_num: UART port number.
 * \param[out]  frame: Pointer to the frame answer structure.
 * \return      Status code of the operation.
 */
int32_t pms7003_read(uart_port_t uart_num, pms7003_frame_answer_t *frame);

#endif // !INC_PMS7003_H
