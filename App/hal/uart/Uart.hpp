/*
 * Uart.hpp
 *
 *  Created on: 6 May 2023
 *      Author: Alexander L.
 */

#ifndef HAL_UART_UART_HPP_
#define HAL_UART_UART_HPP_

#include <cstdint>
#include "main.h"

namespace hal {
namespace uart {

constexpr size_t RxBufferSize = 256;
constexpr size_t TxBufferSize = 256;

class Uart {
 public:
  Uart(UART_HandleTypeDef* uart_handle);
  virtual ~Uart();

  void reset();
  int32_t scheduleTransmit(const uint8_t data[], size_t size);
  int32_t transmit(const uint8_t data[], size_t size);
  int32_t receive(uint8_t data[], size_t max_size);

 private:
  int32_t startTx(const uint8_t data[], size_t size);
  int32_t startRx();

  UART_HandleTypeDef* uart_handle_;
  uint8_t rx_buffer_[RxBufferSize];
  uint8_t tx_buffer_[TxBufferSize];

  uint8_t* pending_tx_start_ = tx_buffer_;
  uint8_t* pending_tx_end_ = tx_buffer_;
  uint8_t* new_tx_start_ = tx_buffer_;
  uint8_t* new_tx_end_ = tx_buffer_;
};

} /* namespace uart */
} /* namespace hal */

#endif /* HAL_UART_UART_HPP_ */
