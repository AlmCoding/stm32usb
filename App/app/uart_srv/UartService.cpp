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

UartService::UartService() : uart1_{ &huart1 } {}

UartService::~UartService() {}

void UartService::init() {
  uart1_.init();
}

void UartService::run() {
  uart1_.transmit();

  if (uart1_.receivedBytes() > 0) {
    // Inform UsbTask to service received data
    os::msg::BaseMsg msg = { .id = os::msg::MsgId::ServiceTxRequest };
    os::msg::send_msg(os::msg::MsgQueue::UsbTaskQueue, &msg);
  }
}

int32_t UartService::getRxRequest(uint8_t* data, size_t max_size) {
  return uart1_.serviceRx(data, max_size);
}

int32_t UartService::postTxRequest(const uint8_t* data, size_t size) {
  int32_t status = -1;

  if (uart1_.scheduleTx(data, size) == StatusType::Ok) {
    status = 0;
  }

  return status;
}

} /* namespace uart_srv */
} /* namespace app */
