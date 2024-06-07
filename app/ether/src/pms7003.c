#include "pms7003.h"
#include "esp_log.h"

static const char *TAG = "pms7003";

static uint16_t convert_to_little_endian(uint16_t data) 
{
  return ((data & 0x00ff) << 8 | (data & 0xff00) >> 8);
}

pms7003_result_t pms7003_frame_send(const pms7003_callback_sent_t handler, uart_port_t uart_num) 
{
  if (!handler) {
    return PMS7003_RESULT_ERROR;
  }

  int32_t bytes_sent = handler(uart_num);

  if (bytes_sent < 0) {
    return PMS7003_RESULT_ERROR;
  } else if (bytes_sent != PMS7003_FRAME_REQUEST_SIZE) {
    return PMS7003_RESULT_PARTIAL_SENT;
  }

  return PMS7003_RESULT_SUCCESS;
}

pms7003_result_t pms7003_frame_receive(const pms7003_callback_received_t handler, 
                                       uart_port_t uart_num, pms7003_frame_answer_t *frame) 
{
  if ((!handler) || (!frame)) {
    return PMS7003_RESULT_ERROR;
  }
  
  int32_t bytes_received = handler(uart_num, frame);
  uint16_t calculated_check_code = 0;

  if (bytes_received < 0) {
    return PMS7003_RESULT_ERROR;
  } else if (bytes_received != PMS7003_FRAME_ANSWER_SIZE) {
    return PMS7003_RESULT_PARTIAL_RECEIVED;
  }

  for (uint8_t i = 0; i < PMS7003_FRAME_CHECK_CODE_SIZE; ++i) {
    calculated_check_code += frame->buffer_answer[i];
  }

#if defined(DEBUG)
  ESP_LOG_BUFFER_HEXDUMP(TAG, frame->buffer_answer, PMS7003_FRAME_ANSWER_SIZE, ESP_LOG_INFO);
  ESP_LOGI(TAG, "calculated: 0x%x", calculated_check_code);
  ESP_LOGI(TAG, "real: 0x%x", convert_to_little_endian(frame->check_code));
#endif

  if (calculated_check_code == convert_to_little_endian(frame->check_code)) {
    return PMS7003_RESULT_SUCCESS;
  } else {
    return PMS7003_RESULT_WRONG_CHECK_CODE;
  }
}

int32_t pms7003_read_request(uart_port_t uart_num) 
{
  pms7003_frame_request_t frame = PMS7003_FRAME_READ;
  return uart_write_bytes(uart_num, frame.buffer_request, PMS7003_FRAME_REQUEST_SIZE);
}

int32_t pms7003_change_mode_passive(uart_port_t uart_num) 
{
  pms7003_frame_request_t frame = PMS7003_FRAME_CHANGE_MODE_PASSIVE;
  return uart_write_bytes(uart_num, frame.buffer_request, PMS7003_FRAME_REQUEST_SIZE);
}

int32_t pms7003_change_mode_active(uart_port_t uart_num) 
{
  pms7003_frame_request_t frame = PMS7003_FRAME_CHANGE_MODE_ACTIVE;
  return uart_write_bytes(uart_num, frame.buffer_request, PMS7003_FRAME_REQUEST_SIZE);
}

int32_t pms7003_sleep(uart_port_t uart_num) 
{
  pms7003_frame_request_t frame = PMS7003_FRAME_SLEEP;
  return uart_write_bytes(uart_num, frame.buffer_request, PMS7003_FRAME_REQUEST_SIZE);
}

int32_t pms7003_wakeup(uart_port_t uart_num) 
{
  pms7003_frame_request_t frame = PMS7003_FRAME_WAKEUP;
  return uart_write_bytes(uart_num, frame.buffer_request, PMS7003_FRAME_REQUEST_SIZE);
}

int32_t pms7003_read(uart_port_t uart_num, pms7003_frame_answer_t *frame) 
{
  int32_t length = 0;

  while (1) {
    length = uart_read_bytes(uart_num, frame->buffer_answer, 2 * PMS7003_FRAME_BYTE_SIZE, 
                             PMS7003_UART_WAIT_TIMEOUT_MS);

    if (length == 2) {
      if ((frame->buffer_answer[0] == PMS7003_START_CHARACTER_1) && 
          (frame->buffer_answer[1] == PMS7003_START_CHARACTER_2)) {
        return uart_read_bytes(uart_num, frame->buffer_answer + length, 
                               PMS7003_FRAME_ANSWER_SIZE - length, PMS7003_UART_WAIT_TIMEOUT_MS) + length;
      }
    }

    length = 0;
  }
}
