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

int32_t UartService::poll() {
  return uart1_.poll();
}

int32_t UartService::postRequest(const uint8_t* data, size_t size) {
  int32_t status = -1;

  /* Allocate space for the decoded message. */
  uart_proto_UartMsg uart_msg = uart_proto_UartMsg_init_zero;
  /* Create a stream that reads from the buffer. */
  pb_istream_t stream = pb_istream_from_buffer(data, size);

  /* Now we are ready to decode the message. */
  if (pb_decode(&stream, uart_proto_UartMsg_fields, &uart_msg) == false) {
    DEBUG_ERROR("ProtoBuf decode [failed]");
    return -1;
  }

  if (uart_msg.which_msg == uart_proto_UartMsg_data_msg_tag) {
    if (uart1_.scheduleTx(static_cast<uint8_t*>(uart_msg.msg.data_msg.data.bytes), uart_msg.msg.data_msg.data.size,
                          uart_msg.sequence_number) == Status_t::Ok) {
      status = 0;
    }
  }

  return status;
}

int32_t UartService::serviceRequest(uint8_t* data, size_t max_size) {
  /* Allocate space for the decoded message. */
  uart_proto_UartMsg uart_msg = uart_proto_UartMsg_init_zero;
  /* Create a stream that will write to our buffer. */
  pb_ostream_t stream = pb_ostream_from_buffer(data, max_size);

  hal::uart::ServiceRequest req;
  uart_msg.sequence_number = uart1_.getServiceRequest(&req);

  if (req == hal::uart::ServiceRequest::SendRxData) {
    serviceDataRequest(&uart_msg, max_size);
  } else if (req == hal::uart::ServiceRequest::SendStatus) {
    serviceStatusRequest(&uart_msg, max_size);
  }

  /* Now we are ready to encode the message! */
  if (pb_encode(&stream, uart_proto_UartMsg_fields, &uart_msg) == false) {
    DEBUG_ERROR("ProtoBuf encode [failed]");
    return -1;
  }

  return stream.bytes_written;
}

void UartService::serviceDataRequest(uart_proto_UartMsg* msg, size_t max_size) {
  msg->which_msg = uart_proto_UartMsg_data_msg_tag;

  size_t data_size = uart1_.serviceRx(msg->msg.data_msg.data.bytes, max_size);
  msg->msg.data_msg.data.size = static_cast<uint16_t>(data_size);
  DEBUG_INFO("Service %d bytes", data_size);
}

void UartService::serviceStatusRequest(uart_proto_UartMsg* msg, size_t /*max_size*/) {
  hal::uart::UartStatus status;
  uart1_.serviceStatus(&status);

  msg->which_msg = uart_proto_UartMsg_status_msg_tag;

  msg->msg.status_msg.rx_overflow = status.rx_overflow;
  msg->msg.status_msg.tx_overflow = status.tx_overflow;
  msg->msg.status_msg.tx_complete = status.tx_complete;
  msg->msg.status_msg.rx_space = status.rx_space;
  msg->msg.status_msg.tx_space = status.tx_space;

  DEBUG_INFO("Service status info (seq: %d)", msg->sequence_number);
}

} /* namespace uart_srv */
} /* namespace app */
