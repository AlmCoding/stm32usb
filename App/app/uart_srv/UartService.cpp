/*
 * UartService.cpp
 *
 *  Created on: May 6, 2023
 *      Author: Alexander L.
 */

#include "app/uart_srv/UartService.hpp"
#include "os/msg/msg_broker.hpp"
#include "usart.h"

namespace app {
namespace uart_srv {

UartService::UartService() : uart1{ &huart1 } {}

UartService::~UartService() {}

void UartService::run() {
  uart1.transmit();

  if (uart1.receivedBytes() > 0) {
    // Inform USB_task to come and get the data
    // os::msg::send_msg(queue, msg);
  }
}

int32_t UartService::forwardTxRequest(const uint8_t* data, size_t size) {
  int32_t status = -1;

  if (uart1.scheduleTransmit(data, size) == StatusType::Ok) {
    status = 0;
  }

  return status;
}

} /* namespace uart_srv */
} /* namespace app */
