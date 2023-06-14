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

#define DMA_RX_WRITE_POS ((RxBufferSize - uart_handle_->hdmarx->Instance->CNDTR) & (RxBufferSize - 1))

Uart::Uart(UART_HandleTypeDef* uart_handle) : uart_handle_{ uart_handle } {}

Uart::~Uart() {}

Status_t Uart::init() {
  free_tx_space_ = sizeof(tx_buffer_);
  next_tx_start_ = 0;
  next_tx_end_ = 0;
  tx_complete_ = true;
  tx_overflow_ = false;

  rx_read_pos_ = 0;
  rx_overflow_ = false;

  send_data_msg_ = false;
  send_status_msg_ = false;

  return startRx();
}

size_t Uart::poll() {
  size_t service_requests = 0;

  transmit();

  if (isRxBufferEmpty() == false) {
    send_data_msg_ = true;
    service_requests++;
  }

  if (send_status_msg_ == true) {
    service_requests++;
  }

  return service_requests;
}

Status_t Uart::startRx() {
  Status_t status;
  HAL_StatusTypeDef rx_status;

  rx_status = HAL_UART_Receive_DMA(uart_handle_, rx_buffer_, sizeof(rx_buffer_));
  if (rx_status == HAL_OK) {
    DEBUG_INFO("Start rx [ok]")
    status = Status_t::Ok;

  } else {
    DEBUG_INFO("Start rx [failed]")
    status = Status_t::Error;
  }

  return status;
}

bool Uart::isRxBufferEmpty() {
  if (rx_read_pos_ == DMA_RX_WRITE_POS) {
    return true;
  }
  return false;
}

Status_t Uart::scheduleTx(const uint8_t* data, size_t size) {
  Status_t status;

  if (free_tx_space_ >= size) {
    DEBUG_INFO("Schedule %d bytes for tx", size)

    size_t new_tx_end;
    size_t tx_size = size;
    size_t size_to_buf_end = sizeof(tx_buffer_) - next_tx_end_;

    if (size > size_to_buf_end) {
      tx_size = size_to_buf_end;
    }

    std::memcpy(tx_buffer_ + next_tx_end_, data, tx_size);
    new_tx_end = next_tx_end_ + tx_size;
    size -= tx_size;

    if (size > 0) {
      std::memcpy(tx_buffer_, data, size);
      new_tx_end = size;
    }

    next_tx_end_ = new_tx_end;
    tx_complete_ = false;
    tx_overflow_ = false;

    // Trigger uart task for fast transmit start
    os::msg::BaseMsg msg = { .id = os::msg::MsgId::TriggerTask };
    os::msg::send_msg(os::msg::MsgQueue::UartTaskQueue, &msg);
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
  HAL_StatusTypeDef tx_status;

  // Recover from error
  if (BITS_SET(uart_handle_->gState, HAL_UART_STATE_ERROR) == true) {
    DEBUG_ERROR("Tx error, try to recover...")
    HAL_UART_DMAStop(uart_handle_);
  }

  // Return if tx busy
  if (BITS_SET(uart_handle_->gState, HAL_UART_STATE_BUSY_TX) == true) {
    return Status_t::Busy;
  }

  // Check for new data
  if (next_tx_end_ != next_tx_start_) {
    uint16_t tx_size;
    size_t new_tx_start;

    if (next_tx_start_ < next_tx_end_) {
      tx_size = static_cast<uint16_t>(next_tx_end_ - next_tx_start_);
      new_tx_start = next_tx_end_;

    } else {
      tx_size = static_cast<uint16_t>(sizeof(tx_buffer_) - next_tx_start_);
      new_tx_start = 0;
    }

    // Start transmit
    DEBUG_INFO("Transmit %d bytes", tx_size);

    tx_status = HAL_UART_Transmit_DMA(uart_handle_, tx_buffer_ + next_tx_start_, tx_size);
    if (tx_status == HAL_OK) {
      next_tx_start_ = new_tx_start;
      DEBUG_INFO("Start tx [ok]")
      status = Status_t::Ok;

    } else {
      DEBUG_INFO("Start tx [failed]")
      status = Status_t::Error;
    }

  } else {
    // Nothing to be done
    status = Status_t::Ok;
  }

  return status;
}

ServiceRequest Uart::getServiceRequest() {
  ServiceRequest req = ServiceRequest::None;

  if (send_data_msg_ == true) {
    req = ServiceRequest::SendRxData;

  } else if (send_status_msg_ == true) {
    req = ServiceRequest::SendStatus;
  }

  return req;
}

size_t Uart::serviceRx(uint8_t* data, size_t max_size) {
  int32_t rx_cnt;
  bool rx_circulation = false;
  size_t rx_write_pos = DMA_RX_WRITE_POS;

  rx_cnt = rx_write_pos - rx_read_pos_;
  if (rx_cnt < 0) {
    rx_cnt = sizeof(rx_buffer_) - rx_read_pos_;
    rx_circulation = true;
  }

  if (rx_cnt > static_cast<int32_t>(max_size)) {
    rx_cnt = max_size;
  }

  std::memcpy(data, rx_buffer_ + rx_read_pos_, rx_cnt);

  if (rx_circulation == true) {
    if ((rx_write_pos + rx_cnt) > max_size) {
      rx_write_pos = max_size - rx_cnt;
    }

    if (rx_write_pos > 0) {
      std::memcpy(data + rx_cnt, rx_buffer_, rx_write_pos);
      rx_cnt += rx_write_pos;
    }
  }

  rx_read_pos_ = rx_write_pos;
  return rx_cnt;
}

void Uart::serviceStatus(UartStatus* status) {
  status->rx_overflow = rx_overflow_;
  status->tx_overflow = tx_overflow_;
  status->tx_complete = tx_complete_;
  status->rx_space = uart_handle_->RxXferCount;
  status->tx_space = free_tx_space_;
  send_status_msg_ = false;
}

} /* namespace hal::uart */
