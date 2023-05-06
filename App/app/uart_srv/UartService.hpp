/*
 * UartService.hpp
 *
 *  Created on: May 6, 2023
 *      Author: Alexander L.
 */

#ifndef APP_UART_SRV_UARTSERVICE_HPP_
#define APP_UART_SRV_UARTSERVICE_HPP_

#include "hal/uart/Uart.hpp"

namespace app {
namespace uart_srv {

class UartService {
 public:
  UartService();
  virtual ~UartService();

  void run();

 private:
  hal::uart::Uart uart1;
};

} /* namespace uart_srv */
} /* namespace app */

#endif /* APP_UART_SRV_UARTSERVICE_HPP_ */
