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
#include "srv/debug.hpp"

#define DEBUG_ENABLE_UART_SERVICE
#ifdef DEBUG_ENABLE_UART_SERVICE
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][uartSrv]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][uartSrv]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][uartSrv]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

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
  uart_proto_UartMsg uart_msg = uart_proto_UartMsg_init_zero;

  /* Create a stream that reads from the buffer. */
  pb_istream_t stream = pb_istream_from_buffer(data, size);

  /* Now we are ready to decode the message. */
  if (pb_decode(&stream, uart_proto_UartMsg_fields, &uart_msg) == false) {
    DEBUG_ERROR("ProtoBuf decode [fail]");
    return -1;
  }

  if (uart_msg.type == uart_proto_MsgType::uart_proto_MsgType_DATA) {
    if (uart1_.scheduleTx(static_cast<uint8_t*>(uart_msg.msg.data.data.bytes), uart_msg.msg.data.data.size) ==
        StatusType::Ok) {
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
