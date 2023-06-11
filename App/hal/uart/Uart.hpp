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

namespace hal {
namespace uart {

constexpr size_t RxBufferSize = 256;
constexpr size_t TxBufferSize = 256;
constexpr size_t RxRestartThreshold = RxBufferSize * 3 / 4;

enum class ServiceRequest {
  None = 0,
  DataService,
  StatusService,
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

  void init();
  size_t poll();

  Status_t scheduleTx(const uint8_t* data, size_t size);
  Status_t transmit();

  ServiceRequest getServiceRequest();
  bool serviceStatus(UartStatus* status);

  size_t receive();
  size_t serviceRx(uint8_t* data, size_t max_size);

 private:
  size_t updateFreeTxSpace();
  Status_t startRx();

  UART_HandleTypeDef* uart_handle_;
  uint8_t rx_buffer_[RxBufferSize];
  uint8_t tx_buffer_[TxBufferSize];

  uint8_t* next_tx_start_ = tx_buffer_;
  uint8_t* next_tx_end_ = tx_buffer_;

  size_t serviced_rx_bytes_ = 0;
  size_t pending_rx_bytes_ = 0;
  size_t free_rx_space_ = RxBufferSize;
  size_t free_tx_space_ = TxBufferSize;

  bool rx_overflow_ = false;
  bool tx_overflow_ = false;
  bool tx_complete_ = true;
  bool send_status_msg_ = false;
};

} /* namespace uart */
} /* namespace hal */

#endif /* HAL_UART_UART_HPP_ */
