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

constexpr size_t RxBufferSize = 128;
constexpr size_t TxBufferSize = 128;

class Uart {
 public:
  Uart(UART_HandleTypeDef* uart_handle);
  virtual ~Uart();

  int32_t transmit(const uint8_t data[], const size_t size);
  int32_t receive(uint8_t data[], const size_t max_size);

 private:
  int32_t startTx(const uint8_t data[], const size_t size);
  int32_t startRx();

  UART_HandleTypeDef* uart_handle_;
  uint8_t rx_buffer_[RxBufferSize];
  uint8_t tx_buffer_[TxBufferSize];
};

} /* namespace uart */
} /* namespace hal */

#endif /* HAL_UART_UART_HPP_ */
