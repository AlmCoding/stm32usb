/*
 * Uart.cpp
 *
 *  Created on: 6 May 2023
 *      Author: Alexander L.
 */

#include "hal/uart/Uart.hpp"
#include "hal/uart/UartIrq.hpp"
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
#define DMA_TX_READ_POS (uart_handle_->hdmatx->Instance->CMAR - reinterpret_cast<uint32_t>(tx_buffer_))

Uart::Uart(UART_HandleTypeDef* uart_handle) : uart_handle_{ uart_handle } {
  UartIrq::getInstance().registerUart(this);
}

Uart::~Uart() {}

Status_t Uart::init() {
  this_tx_start_ = 0;
  next_tx_start_ = 0;
  next_tx_end_ = 0;
  tx_complete_ = true;
  tx_overflow_ = false;

  rx_read_pos_ = 0;
  rx_overflow_ = false;

  send_data_msg_ = false;
  send_status_msg_ = false;
  seqence_number_ = 0;

  return startRx();
}

size_t Uart::poll() {
  size_t service_requests = 0;

  /*
  // Recover from error
  if (BITS_SET(uart_handle_->gState, HAL_UART_STATE_ERROR) == true) {
    DEBUG_ERROR("Tx error, try to recover...")
    HAL_UART_DMAStop(uart_handle_);
  }
  */

  // startTx();

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

size_t Uart::getFreeTxSpace(size_t seq_num) {
  // From next_tx_end_ to DMA_TX_READ_POS
  int32_t dma_tx_write_pos = DMA_TX_READ_POS;
  int32_t next_tx_end = next_tx_end_;
  int32_t free_tx_space = sizeof(tx_buffer_);

  if (tx_complete_ == false) {
    if (dma_tx_write_pos != next_tx_end) {
      free_tx_space += dma_tx_write_pos - next_tx_end;

      if (dma_tx_write_pos > next_tx_end) {
        free_tx_space -= sizeof(tx_buffer_);
      }
    }

  } else {
    dma_tx_write_pos = -1;
  }

  DEBUG_INFO("[start: %d, dma: %d, end: %d]", this_tx_start_, dma_tx_write_pos, next_tx_start_)
  DEBUG_INFO("Free tx space: %d (seq: %d)", free_tx_space, seq_num)
  return free_tx_space;
}

Status_t Uart::scheduleTx(const uint8_t* data, size_t size, size_t seq_num) {
  Status_t status;

  if (getFreeTxSpace(seq_num) >= size) {
    DEBUG_INFO("Schedule tx (size: %d, seq: %d)", size, seq_num)

    size_t new_tx_end;
    size_t tx_size = size;
    size_t size_to_buf_end = sizeof(tx_buffer_) - next_tx_end_;

    if (size > size_to_buf_end) {
      tx_size = size_to_buf_end;
    }

    std::memcpy(tx_buffer_ + next_tx_end_, data, tx_size);
    size -= tx_size;

    if (tx_size < size_to_buf_end) {
      new_tx_end = next_tx_end_ + tx_size;
    } else {
      new_tx_end = 0;
    }

    if (size > 0) {
      std::memcpy(tx_buffer_, data, size);
      new_tx_end = size;
    }

    next_tx_end_ = new_tx_end;
    tx_complete_ = false;
    tx_overflow_ = false;

    startTx();

    // Trigger uart task for fast transmit start
    os::msg::BaseMsg msg = { .id = os::msg::MsgId::TriggerTask };
    os::msg::send_msg(os::msg::MsgQueue::UartTaskQueue, &msg);
    status = Status_t::Ok;

  } else {
    DEBUG_ERROR("Tx overflow (size: %d, seq: %d)", size, seq_num);
    tx_overflow_ = true;
    send_status_msg_ = true;
    status = Status_t::Error;
  }

  seqence_number_ = seq_num;
  return status;
}

Status_t Uart::startTx() {
  Status_t status;
  HAL_StatusTypeDef tx_status;

  // Return if tx busy
  if (BITS_SET(uart_handle_->gState, HAL_UART_STATE_BUSY_TX) == true) {
    return Status_t::Busy;
  }

  // Check for new data
  if (next_tx_end_ != next_tx_start_) {
    uint16_t tx_size;
    size_t new_tx_start;

    // Protect against change due to new scheduleTx (interrupt context)
    size_t next_tx_end = next_tx_end_;

    if (next_tx_start_ < next_tx_end) {
      tx_size = static_cast<uint16_t>(next_tx_end - next_tx_start_);
      new_tx_start = next_tx_end;

    } else {
      tx_size = static_cast<uint16_t>(sizeof(tx_buffer_) - next_tx_start_);
      new_tx_start = 0;
    }

    // Start transmit
    tx_status = HAL_UART_Transmit_DMA(uart_handle_, tx_buffer_ + next_tx_start_, tx_size);
    if (tx_status == HAL_OK) {
      this_tx_start_ = next_tx_start_;
      next_tx_start_ = new_tx_start;

      DEBUG_INFO("Start tx (size: %d) [ok]", tx_size);
      DEBUG_INFO("[start: %d, dma: %d, end: %d]", this_tx_start_, DMA_TX_READ_POS, next_tx_start_)
      status = Status_t::Ok;

    } else {
      DEBUG_ERROR("Start tx (size: %d) [failed]", tx_size);
      status = Status_t::Error;
    }

  } else {
    // Nothing to be done
    status = Status_t::Ok;
  }

  return status;
}

void Uart::txCpltCallback() {
  DEBUG_INFO("Tx cplt");

  // Check for new data
  if (next_tx_end_ != next_tx_start_) {
    startTx();

  } else {
    this_tx_start_ = 0;
    next_tx_start_ = 0;
    next_tx_end_ = 0;
    tx_complete_ = true;
  }

  send_status_msg_ = true;
}

size_t Uart::getServiceRequest(ServiceRequest* req) {
  *req = ServiceRequest::None;

  if (send_data_msg_ == true) {
    *req = ServiceRequest::SendRxData;

  } else if (send_status_msg_ == true) {
    send_status_msg_ = false;
    *req = ServiceRequest::SendStatus;
  }

  return seqence_number_;
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
  status->tx_complete = tx_complete_;
  status->tx_overflow = tx_overflow_;
  status->tx_space = getFreeTxSpace(seqence_number_);

  status->rx_overflow = rx_overflow_;
  status->rx_space = 0;  // uart_handle_->RxXferCount;
}

} /* namespace hal::uart */
