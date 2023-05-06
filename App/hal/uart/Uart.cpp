/*
 * Uart.cpp
 *
 *  Created on: 6 May 2023
 *      Author: Alexander L.
 */

#include "hal/uart/Uart.hpp"
#include <cstring>
#include "os/msg/send_msg.hpp"

namespace hal {
namespace uart {

Uart::Uart(UART_HandleTypeDef* uart_handle) : uart_handle_{ uart_handle } {
  // Start recording incoming data
  startRx();
}

Uart::~Uart() {}

void Uart::reset() {
  HAL_UART_Abort(uart_handle_);

  pending_tx_start_ = tx_buffer_;
  pending_tx_end_ = tx_buffer_;
  new_tx_start_ = tx_buffer_;
  new_tx_end_ = tx_buffer_;
}

int32_t Uart::scheduleTransmit(const uint8_t data[], size_t size) {
  int32_t status;
  size_t free_space = tx_buffer_ + TxBufferSize - new_tx_end_;

  if (free_space >= size) {
    std::memcpy(new_tx_end_, data, size);
    new_tx_end_ += size;

    os::msg::MsgType msg = { .id = os::msg::MsgId::ServiceTxUart1 };
    os::msg::send_msg_isr(os::msg::MsgQueue::UartTaskQueue, msg);
    status = 0;

  } else {
    // Not enough free space in buffer (drop entire frame)
    status = -1;
  }

  return status;
}

int32_t Uart::transmit(const uint8_t data[], size_t size) {
  int32_t status = -1;

  if (uart_handle_->gState == HAL_UART_STATE_ERROR) {
    HAL_UART_AbortTransmit(uart_handle_);
    status = startTx(data, size);

  } else if (uart_handle_->gState == HAL_UART_STATE_READY) {
    status = startTx(data, size);

  } else {
    status = 1;  // Busy
  }

  return status;
}

int32_t Uart::receive(uint8_t data[], size_t max_size) {
  int32_t rx_cnt = 0;

  if (uart_handle_->RxXferCount > 0) {
    // Data available
    if (uart_handle_->RxXferCount > max_size) {
      rx_cnt = max_size;
    } else {
      rx_cnt = uart_handle_->RxXferCount;
    }

    std::memcpy(data, rx_buffer_, rx_cnt);

    if (startRx() == -1) {
      rx_cnt = -1;  // Error
    }
  }

  return rx_cnt;
}

int32_t Uart::startTx(const uint8_t data[], size_t size) {
  int32_t status = -1;
  HAL_StatusTypeDef hal_sts;

  if (size > TxBufferSize) {
    size = TxBufferSize;
  }

  std::memcpy(tx_buffer_, data, size);
  hal_sts = HAL_UART_Transmit_IT(uart_handle_, tx_buffer_, size);

  if (hal_sts == HAL_OK) {
    status = 0;
  }

  return status;
}

int32_t Uart::startRx() {
  int32_t status = -1;
  HAL_StatusTypeDef hal_sts;

  if (uart_handle_->gState == HAL_UART_STATE_ERROR) {
    HAL_UART_AbortReceive(uart_handle_);
  }

  hal_sts = HAL_UART_Receive_IT(uart_handle_, rx_buffer_, RxBufferSize);

  if (hal_sts == HAL_OK) {
    status = 0;
  }

  return status;
}

} /* namespace uart */
} /* namespace hal */
