/*
 * Uart.hpp
 *
 *  Created on: 6 May 2023
 *      Author: Alexander L.
 */

#ifndef HAL_UART_UART_HPP_
#define HAL_UART_UART_HPP_

#include <stdint.h>
#include "lib/etl/vector.h"

namespace hal {
namespace uart {

class Uart {
 public:
  Uart();

  int32_t transmit(const uint8_t data[], const uint16_t size);
  int32_t receive(uint8_t data[], const uint16_t max_size);

  virtual ~Uart();

 private:
  etl::vector<uint8_t, 128> tx_buffer_;
  etl::vector<uint8_t, 128> rx_buffer_;
};

} /* namespace uart */
} /* namespace hal */

#endif /* HAL_UART_UART_HPP_ */
