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
#include "usart.h"

namespace app::uart_srv {

class UartService {
 public:
  UartService();
  virtual ~UartService();

  void init();
  uint32_t poll();

  int32_t postRequest(const uint8_t* data, size_t len);
  int32_t serviceRequest(uint8_t* data, size_t max_len);

 private:
  void serviceDataRequest(uart_proto_UartMsg* msg, size_t max_len);
  void serviceStatusRequest(uart_proto_UartMsg* msg, size_t max_len);

  hal::uart::Uart uart1_{ &huart1 };
};

}  // namespace app::uart_srv

#endif /* APP_UART_SRV_UARTSERVICE_HPP_ */
