/*
 * Uart.hpp
 *
 *  Created on: 6 May 2023
 *      Author: Alexander L.
 */

#ifndef HAL_UART_UART_HPP_
#define HAL_UART_UART_HPP_

#include "common.hpp"
#include "main.h"

namespace hal::uart {

constexpr size_t RxBufferSize = 128;
constexpr size_t TxBufferSize = 128;

enum class ServiceRequest {
  None = 0,
  SendRxData,
  SendStatus,
};

typedef struct {
  bool rx_overflow;
  bool tx_overflow;
  bool tx_complete;
  size_t rx_space;
  size_t tx_space;
} UartStatus;

class Uart {
 public:
  Uart(UART_HandleTypeDef* uart_handle);
  virtual ~Uart();

  Status_t init();
  size_t poll();

  Status_t scheduleTx(const uint8_t* data, size_t size);

  ServiceRequest getServiceRequest();
  size_t serviceRx(uint8_t* data, size_t max_size);
  void serviceStatus(UartStatus* status);

 private:
  Status_t startRx();
  bool isRxBufferEmpty();
  Status_t transmit();

  UART_HandleTypeDef* uart_handle_;
  uint8_t rx_buffer_[RxBufferSize];
  uint8_t tx_buffer_[TxBufferSize];

  size_t free_tx_space_ = sizeof(tx_buffer_);
  size_t next_tx_start_ = 0;
  size_t next_tx_end_ = 0;
  bool tx_complete_ = true;
  bool tx_overflow_ = false;

  size_t rx_read_pos_ = 0;
  bool rx_overflow_ = false;

  bool send_data_msg_ = false;
  bool send_status_msg_ = false;
};

} /* namespace hal::uart */

#endif /* HAL_UART_UART_HPP_ */
