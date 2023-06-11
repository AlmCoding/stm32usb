/*
 * Uart.cpp
 *
 *  Created on: 6 May 2023
 *      Author: Alexander L.
 */

#include "hal/uart/Uart.hpp"
#include "os/msg/msg_broker.hpp"
#include "srv/debug.hpp"

#define DEBUG_ENABLE_UART
#ifdef DEBUG_ENABLE_UART
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][Uart]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][Uart]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][Uart]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace hal::uart {

Uart::Uart(UART_HandleTypeDef* uart_handle) : uart_handle_{ uart_handle } {}

Uart::~Uart() {}

void Uart::init() {
  serviced_rx_bytes_ = 0;
  pending_rx_bytes_ = 0;
  free_rx_space_ = RxBufferSize;
  free_tx_space_ = TxBufferSize;

  next_tx_start_ = tx_buffer_;
  next_tx_end_ = tx_buffer_;

  rx_overflow_ = false;
  tx_overflow_ = false;
  tx_complete_ = true;
  send_status_msg_ = false;

  startRx();
}

size_t Uart::poll() {
  size_t service_requests = 0;

  transmit();
  if (receive() > 0) {
    service_requests++;
  }

  if (send_status_msg_ == true) {
    service_requests++;
  }

  return service_requests;
}

size_t Uart::updateFreeTxSpace() {
  free_tx_space_ = tx_buffer_ + TxBufferSize - next_tx_end_;
  return free_tx_space_;
}

Status_t Uart::scheduleTx(const uint8_t* data, size_t size) {
  Status_t status;

  if (updateFreeTxSpace() >= size) {
    DEBUG_INFO("Schedule %d bytes for tx", size)
    std::memcpy(next_tx_end_, data, size);
    next_tx_end_ += size;

    // Trigger uart task for fast transmit start
    os::msg::BaseMsg msg = { .id = os::msg::MsgId::TriggerTask };
    os::msg::send_msg(os::msg::MsgQueue::UartTaskQueue, &msg);

    tx_complete_ = false;
    tx_overflow_ = false;
    status = Status_t::Ok;

  } else {
    DEBUG_ERROR("Tx buffer overflow!");
    tx_overflow_ = true;
    send_status_msg_ = true;
    status = Status_t::Error;
  }

  return status;
}

Status_t Uart::transmit() {
  Status_t status;
  HAL_StatusTypeDef hal_sts = HAL_ERROR;

  // Recover from error
  if (BITS_SET(uart_handle_->gState, HAL_UART_STATE_ERROR) == true) {
    DEBUG_ERROR("Tx error state")
    HAL_UART_AbortTransmit(uart_handle_);
  }

  // Return if busy
  if (BITS_SET(uart_handle_->gState, HAL_UART_STATE_BUSY_TX) == true) {
    return Status_t::Busy;
  }

  // Check for new data
  if (next_tx_end_ != next_tx_start_) {
    uint16_t size = static_cast<uint16_t>(next_tx_end_ - next_tx_start_);

    // Start transmit
    DEBUG_INFO("Transmit %d bytes", size)
    hal_sts = HAL_UART_Transmit_IT(uart_handle_, next_tx_start_, size);

    if (hal_sts == HAL_OK) {
      next_tx_start_ = next_tx_end_;
      status = Status_t::Ok;
    } else {
      status = Status_t::Error;
    }

  } else if (next_tx_start_ != tx_buffer_) {
    // No new data and uart ready => reset tx buffer
    DEBUG_INFO("Tx complete")
    next_tx_start_ = tx_buffer_;
    next_tx_end_ = tx_buffer_;
    updateFreeTxSpace();
    tx_complete_ = true;
    send_status_msg_ = true;
    status = Status_t::Ok;

  } else {
    // Nothing to be done
    status = Status_t::Ok;
  }

  return status;
}

ServiceRequest Uart::getServiceRequest() {
  ServiceRequest req = ServiceRequest::None;

  if (pending_rx_bytes_ > 0) {
    req = ServiceRequest::DataService;
  } else if (send_status_msg_ == true) {
    req = ServiceRequest::StatusService;
  }

  return req;
}

bool Uart::serviceStatus(UartStatus* status) {
  bool send_msg = send_status_msg_;

  if (send_status_msg_ == true) {
    status->rx_overflow = rx_overflow_;
    status->tx_overflow = tx_overflow_;
    status->tx_complete = tx_complete_;
    status->rx_space = free_rx_space_;
    status->tx_space = free_tx_space_;
    send_status_msg_ = false;
  }

  return send_msg;
}

size_t Uart::receive() {
  pending_rx_bytes_ = uart_handle_->RxXferSize - uart_handle_->RxXferCount - serviced_rx_bytes_;
  free_rx_space_ = uart_handle_->RxXferCount;

  if ((pending_rx_bytes_ == 0) && (free_rx_space_ <= RxRestartThreshold)) {
    startRx();
    rx_overflow_ = false;

  } else if (pending_rx_bytes_ >= uart_handle_->RxXferSize) {
    DEBUG_ERROR("Rx buffer overflow!")
    rx_overflow_ = true;
    send_status_msg_ = true;
  }

  return pending_rx_bytes_;
}

size_t Uart::serviceRx(uint8_t* data, size_t max_size) {
  size_t rx_cnt = pending_rx_bytes_;

  if (rx_cnt > max_size) {
    rx_cnt = max_size;
  }

  if (rx_cnt > 0) {
    std::memcpy(data, (rx_buffer_ + serviced_rx_bytes_), rx_cnt);
    serviced_rx_bytes_ += rx_cnt;
    pending_rx_bytes_ -= rx_cnt;
  }

  return rx_cnt;
}

Status_t Uart::startRx() {
  Status_t status;
  HAL_StatusTypeDef hal_sts;

  serviced_rx_bytes_ = 0;
  HAL_UART_AbortReceive(uart_handle_);

  hal_sts = HAL_UART_Receive_IT(uart_handle_, rx_buffer_, RxBufferSize);
  if (hal_sts == HAL_OK) {
    DEBUG_INFO("Restart rx [ok]")
    status = Status_t::Ok;

  } else {
    DEBUG_ERROR("Restart rx [failed]")
    status = Status_t::Error;
  }

  return status;
}

} /* namespace hal::uart */
