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
#include "os/mutex.hpp"
#include "os/task.hpp"
#include "srv/debug.hpp"

#define DEBUG_ENABLE_UART_TASK
#ifdef DEBUG_ENABLE_UART_TASK
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][uartTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][uartTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][uartTsk]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace task::uart {

void uartTask_requestService_cb(os::msg::RequestCnt cnt);
int32_t uartTask_postRequest_cb(const uint8_t* data, size_t size);
int32_t uartTask_serviceRequest_cb(uint8_t* data, size_t max_size);

constexpr app::usb::UsbMsgType TaskUsbMsgType = app::usb::UsbMsgType::UartMsg;
static app::uart_srv::UartService uart_service_{};
static bool ongoing_service_ = false;
static uint32_t msg_count_ = 0;

void uartTask(void* /*argument*/) {
  os::msg::BaseMsg msg;

  // Initialize service with notification callback
  uart_service_.init(uartTask_requestService_cb);

  // Register callback for incoming msg
  driver::tf::FrameDriver::getInstance().registerRxCallback(TaskUsbMsgType, uartTask_postRequest_cb);

  // Register callback for outgoing msg
  driver::tf::FrameDriver::getInstance().registerTxCallback(TaskUsbMsgType, uartTask_serviceRequest_cb);

  /* Infinite loop */
  for (;;) {
    if (os::msg::receive_msg(os::msg::MsgQueue::UartTaskQueue, &msg, os::CycleTime_UartTask) == true) {
      // process msg
    }

    uart_service_.poll();
  }
}

void uartTask_requestService_cb(os::msg::RequestCnt cnt) {
  if (ongoing_service_ == true) {
    return;
  }
  ongoing_service_ = true;

  os::msg::BaseMsg req_msg = {
    .id = os::msg::MsgId::ServiceUpstreamRequest,
    .type = TaskUsbMsgType,
    .cnt = cnt,
  };

  if (os::msg::send_msg(os::msg::MsgQueue::CtrlTaskQueue, &req_msg) == true) {
    DEBUG_INFO("Notify ctrlTask: %d [ok]", ++msg_count_)
  } else {
    DEBUG_ERROR("Notify ctrlTask: %d [failed]", ++msg_count_)
  }
}

int32_t uartTask_postRequest_cb(const uint8_t* data, size_t len) {
  return uart_service_.postRequest(data, len);
}

int32_t uartTask_serviceRequest_cb(uint8_t* data, size_t max_len) {
  ongoing_service_ = false;
  int32_t len = uart_service_.serviceRequest(data, max_len);

  DEBUG_INFO("Service request: %d [ok]", msg_count_)
  return len;
}

}  // namespace task::uart
