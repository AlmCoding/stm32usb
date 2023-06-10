/*
 * uart_task.cpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#include "task/uartTask.hpp"

#include "app/uart_srv/UartService.hpp"
#include "cmsis_os.h"
#include "driver/tf/FrameDriver.hpp"
#include "os/msg/msg_broker.hpp"
#include "os/task.hpp"
#include "srv/debug.hpp"

#define DEBUG_ENABLE_UART_TASK
#ifdef DEBUG_ENABLE_UART_TASK
#define DEBUG_INFO(f, ...) srv::debug::print(srv::debug::TERM0, "[INF][uartTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::debug::print(srv::debug::TERM0, "[WRN][uartTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::debug::print(srv::debug::TERM0, "[ERR][uartTsk]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace task::uart {

int32_t uartTask_postRequest(const uint8_t* data, size_t size);
int32_t uartTask_serviceRequest(uint8_t* data, size_t max_size);

static app::uart_srv::UartService uart_service_{};
static bool pending_request_ = false;

void uartTask(void* /*argument*/) {
  constexpr app::usb::UsbMsgType TaskUsbMsgType = app::usb::UsbMsgType::UartMsg;
  static os::msg::BaseMsg msg;

  uart_service_.init();

  // Register callback for incoming msg
  driver::tf::FrameDriver::getInstance().registerRxCallback(TaskUsbMsgType, uartTask_postRequest);

  // Register callback for outgoing msg
  driver::tf::FrameDriver::getInstance().registerTxCallback(TaskUsbMsgType, uartTask_serviceRequest);

  /* Infinite loop */
  for (;;) {
    if (os::msg::receive_msg(os::msg::MsgQueue::UartTaskQueue, &msg, os::CycleTime_UartTask) == true) {
      // process msg
    }

    int32_t rx_buffer_level = uart_service_.run();
    if ((rx_buffer_level > 0) && (pending_request_ == false)) {
      DEBUG_INFO("Request service (%d bytes)", rx_buffer_level)
      // Inform UsbTask to service received data
      os::msg::BaseMsg req_msg = { .id = os::msg::MsgId::ServiceTxRequest, .type = TaskUsbMsgType };
      os::msg::send_msg(os::msg::MsgQueue::CtrlTaskQueue, &req_msg);
      pending_request_ = true;
    }
  }
}

int32_t uartTask_postRequest(const uint8_t* data, size_t size) {
  return uart_service_.postRequest(data, size);
}

int32_t uartTask_serviceRequest(uint8_t* data, size_t max_size) {
  int32_t size = uart_service_.serviceRequest(data, max_size);
  pending_request_ = false;
  return size;
}

}  // namespace task::uart
