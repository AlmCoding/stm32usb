/*
 * UartService.cpp
 *
 *  Created on: May 6, 2023
 *      Author: Alexander L.
 */

#include "app/uart_srv/UartService.hpp"
#include "os/msg/msg_broker.hpp"
#include "usart.h"

#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "proto_c/uart.pb.h"

namespace app {
namespace uart_srv {

UartService::UartService() : uart1_{ &huart1 } {}

UartService::~UartService() {}

void UartService::init() {
  uart1_.init();
}

int32_t UartService::run() {
  uart1_.transmit();
  return uart1_.receive();
}

int32_t UartService::postRequest(const uint8_t* data, size_t size) {
  int32_t status = -1;

  /* Allocate space for the decoded message. */
  uart_proto_UartData uartData = uart_proto_UartData_init_zero;

  /* Create a stream that reads from the buffer. */
  pb_istream_t stream = pb_istream_from_buffer(data, size);

  /* Now we are ready to decode the message. */
  bool success = pb_decode(&stream, uart_proto_UartData_fields, &uartData);

  if (success == true) {
    if (uart1_.scheduleTx(static_cast<uint8_t*>(uartData.data.bytes), uartData.data.size) == StatusType::Ok) {
      status = 0;
    }
  }

  return status;
}

int32_t UartService::serviceRequest(uint8_t* data, size_t max_size) {
  return uart1_.serviceRx(data, max_size);
}

} /* namespace uart_srv */
} /* namespace app */
