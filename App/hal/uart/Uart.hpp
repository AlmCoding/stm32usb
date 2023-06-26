/*
 * Uart.hpp
 *
 *  Created on: 6 May 2023
 *      Author: Alexander L.
 */

#ifndef HAL_UART_UART_HPP_
#define HAL_UART_UART_HPP_

#include "common.hpp"
#include "main.h"

namespace hal::uart {

#define START_TX_IMMEDIATELY false
constexpr size_t RxBufferSize = 64;
constexpr size_t TxBufferSize = 64 + 1;

enum class ServiceRequest {
  None = 0,
  SendRxData,
  SendStatus,
};

typedef struct {
  bool rx_overflow;
  bool tx_overflow;
  bool tx_complete;
  size_t rx_space;
  size_t tx_space;
} UartStatus;

class Uart {
 public:
  Uart(UART_HandleTypeDef* uart_handle);
  virtual ~Uart();

  Status_t init();
  uint32_t poll();

  Status_t scheduleTx(const uint8_t* data, size_t len, uint32_t seq_num);

  uint32_t getServiceRequest(ServiceRequest* req);
  size_t serviceRx(uint8_t* data, size_t max_len);
  void serviceStatus(UartStatus* status);

 private:
  Status_t startRx();
  bool isRxBufferEmpty();

  size_t getFreeTxSpace(uint32_t seq_num);
  Status_t startTx();
  void txCpltCallback();

  UART_HandleTypeDef* uart_handle_;
  uint8_t rx_buffer_[RxBufferSize];
  uint8_t tx_buffer_[TxBufferSize];

  size_t this_tx_start_ = 0;
  size_t next_tx_start_ = 0;
  size_t next_tx_end_ = 0;
  bool tx_complete_ = true;
  bool tx_overflow_ = false;

  size_t rx_read_pos_ = 0;
  bool rx_overflow_ = false;

  bool send_data_msg_ = false;
  bool send_status_msg_ = false;
  uint32_t seqence_number_ = 0;

  friend class UartIrq;
};

} /* namespace hal::uart */

#endif /* HAL_UART_UART_HPP_ */
