/*
 * UartIrq.cpp
 *
 *  Created on: 17 Jun 2023
 *      Author: Alexander L.
 */

#include "hal/uart/UartIrq.hpp"
#include "srv/debug.hpp"

#define DEBUG_ENABLE_UART_IRQ
#ifdef DEBUG_ENABLE_UART_IRQ
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][UartIrq]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][UartIrq]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][UartIrq]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace hal::uart {

UartIrq::UartIrq() {}

Status_t UartIrq::registerUart(Uart* uart) {
  Status_t status = Status_t::Error;

  if ((uart != nullptr) && (registered_ < sizeof(uart_))) {
    DEBUG_INFO("Register txCpltCallback (%d) [ok]", registered_)
    uart_[registered_] = uart;
    registered_++;
    status = Status_t::Ok;
  }

  return status;
}

void UartIrq::txCpltCallback(UART_HandleTypeDef* huart) {
  for (size_t i = 0; i < sizeof(uart_); i++) {
    if (uart_[i]->uart_handle_ == huart) {
      uart_[i]->txCpltCallback();
    }
  }
}

void UartIrq::rxTimeoutCallback(UART_HandleTypeDef* huart) {
  for (size_t i = 0; i < sizeof(uart_); i++) {
    if (uart_[i]->uart_handle_ == huart) {
      uart_[i]->rxTimeoutCallback();
    }
  }
}

extern "C" {
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
  UartIrq::getInstance().txCpltCallback(huart);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t /*Size*/) {
  UartIrq::getInstance().rxTimeoutCallback(huart);
}
}  // extern "C"

}  // namespace hal::uart
