/*
 * UartService.hpp
 *
 *  Created on: May 6, 2023
 *      Author: Alexander L.
 */

#ifndef APP_UART_SRV_UARTSERVICE_HPP_
#define APP_UART_SRV_UARTSERVICE_HPP_

#include "hal/uart/Uart.hpp"
#include "proto_c/uart.pb.h"

namespace app {
namespace uart_srv {

class UartService {
 public:
  UartService();
  virtual ~UartService();
  void init();

  int32_t poll();
  int32_t postRequest(const uint8_t* data, size_t size);
  int32_t serviceRequest(uint8_t* data, size_t max_size);

 private:
  void serviceDataRequest(uart_proto_UartMsg* msg, size_t max_size);
  void serviceStatusRequest(uart_proto_UartMsg* msg, size_t max_size);

  hal::uart::Uart uart1_;
};

} /* namespace uart_srv */
} /* namespace app */

#endif /* APP_UART_SRV_UARTSERVICE_HPP_ */
