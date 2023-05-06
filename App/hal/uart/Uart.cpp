/*
 * Uart.cpp
 *
 *  Created on: 6 May 2023
 *      Author: Alexander L.
 */

#include "hal/uart/Uart.hpp"

namespace hal {
namespace uart {

Uart::Uart() {
  tx_buffer_.clear();
  rx_buffer_.clear();
}

Uart::~Uart() {}

} /* namespace uart */
} /* namespace hal */
