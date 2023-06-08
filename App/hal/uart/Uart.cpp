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
#define DEBUG_INFO(f, ...) srv::debug::print(srv::debug::TERM0, "[INF][Uart]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::debug::print(srv::debug::TERM0, "[WRN][Uart]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::debug::print(srv::debug::TERM0, "[ERR][Uart]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace hal::uart {

Uart::Uart(UART_HandleTypeDef* uart_handle) : uart_handle_{ uart_handle } {}

Uart::~Uart() {}

void Uart::init() {
  bytes_serviced_ = 0;
  next_tx_start_ = tx_buffer_;
  next_tx_end_ = tx_buffer_;

  startRx();
}

StatusType Uart::scheduleTx(const uint8_t* data, size_t size) {
  StatusType status = StatusType::Error;
  size_t free_space = tx_buffer_ + TxBufferSize - next_tx_end_;

  if (free_space >= size) {
    std::memcpy(next_tx_end_, data, size);
    next_tx_end_ += size;

    os::msg::BaseMsg msg = { .id = os::msg::MsgId::TriggerTask };
    os::msg::send_msg(os::msg::MsgQueue::UartTaskQueue, &msg);
    status = StatusType::Ok;

  } else {
    DEBUG_ERROR("Not enough free space in buffer");
    status = StatusType::Error;
  }

  return status;
}

StatusType Uart::transmit() {
  StatusType status = StatusType::Error;
  HAL_StatusTypeDef hal_sts = HAL_ERROR;
  uint16_t size = 0;

  // Recover from error
  if (BITS_SET(uart_handle_->gState, HAL_UART_STATE_ERROR) == true) {
    HAL_UART_AbortTransmit(uart_handle_);
  }

  if (BITS_NOT_SET(uart_handle_->gState, HAL_UART_STATE_BUSY_TX) == true) {
    // Check for new data
    if (next_tx_end_ != next_tx_start_) {
      size = static_cast<uint16_t>(next_tx_end_ - next_tx_start_);

      // Start transmit
      hal_sts = HAL_UART_Transmit_IT(uart_handle_, next_tx_start_, size);

      if (hal_sts == HAL_OK) {
        next_tx_start_ = next_tx_end_;
        status = StatusType::Ok;
      } else {
        status = StatusType::Error;
      }

    } else if (next_tx_start_ != tx_buffer_) {
      // No new data and uart ready
      next_tx_start_ = tx_buffer_;
      next_tx_end_ = tx_buffer_;
      status = StatusType::Ok;

    } else {
      // Nothing to be done
      status = StatusType::Ok;
    }

  } else {
    // Busy
    status = StatusType::Busy;
  }

  return status;
}

int32_t Uart::receivedBytes() {
  return (uart_handle_->RxXferSize - uart_handle_->RxXferCount - bytes_serviced_);
}

int32_t Uart::serviceRx(uint8_t* data, size_t max_size) {
  int32_t rx_cnt = receivedBytes();

  if (rx_cnt > 0) {
    if (rx_cnt > static_cast<int32_t>(max_size)) {
      rx_cnt = max_size;
    }

    std::memcpy(data, (rx_buffer_ + bytes_serviced_), rx_cnt);
    bytes_serviced_ += rx_cnt;

    if (uart_handle_->RxXferCount < RxRestartThreshold) {
      // RX buffer filled over restart threshold
      if (rx_cnt == receivedBytes()) {
        // No bytes were received since beginning of this function
        if (startRx() == StatusType::Error) {
          DEBUG_ERROR("Restart RX failed!");
          rx_cnt = -1;  // Error
        }
      }
    }
  }

  return rx_cnt;
}

StatusType Uart::startRx() {
  StatusType status = StatusType::Error;
  HAL_StatusTypeDef hal_sts = HAL_ERROR;

  bytes_serviced_ = 0;
  HAL_UART_AbortReceive(uart_handle_);

  hal_sts = HAL_UART_Receive_IT(uart_handle_, rx_buffer_, RxBufferSize);
  if (hal_sts == HAL_OK) {
    status = StatusType::Ok;
  }

  return status;
}

} /* namespace hal::uart */
