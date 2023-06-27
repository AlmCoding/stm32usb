/*
 * UartConfig.hpp
 *
 *  Created on: 26 Jun 2023
 *      Author: Alexander L.
 */

#ifndef HAL_UART_UARTCONFIG_HPP_
#define HAL_UART_UARTCONFIG_HPP_

namespace hal::uart {

class UartConfig {
 public:
  UartConfig();
  virtual ~UartConfig();
};

}  // namespace hal::uart

#endif /* HAL_UART_UARTCONFIG_HPP_ */
