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
  void init();

  bool run();
  int32_t postRequest(const uint8_t* data, size_t size);
  int32_t serviceRequest(uint8_t* data, size_t max_size);

 private:
  hal::uart::Uart uart1_;
};

} /* namespace uart_srv */
} /* namespace app */

#endif /* APP_UART_SRV_UARTSERVICE_HPP_ */
