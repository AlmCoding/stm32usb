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

namespace task {

int32_t uartTask_postRequest(const uint8_t* data, size_t size);
int32_t uartTask_getRequest(uint8_t* data, size_t max_size);

static app::uart_srv::UartService uart_service_{};

void uartTask(void* /*argument*/) {
  constexpr app::usb::UsbMsgType TaskUsbMsgType = app::usb::UsbMsgType::UartMsg;
  static os::msg::BaseMsg msg;

  uart_service_.init();

  // Register callback for incoming msg
  driver::tf::FrameDriver::getInstance().registerRxCallback(TaskUsbMsgType, uartTask_postRequest);

  // Register callback for outgoing msg
  driver::tf::FrameDriver::getInstance().registerTxCallback(TaskUsbMsgType, uartTask_getRequest);

  /* Infinite loop */
  for (;;) {
    if (os::msg::receive_msg(os::msg::MsgQueue::UartTaskQueue, &msg, os::CycleTime_UartTask) == true) {
      // process msg
    }

    if (uart_service_.run() == true) {
      // Inform UsbTask to service received data
      os::msg::BaseMsg msg = {
        .id = os::msg::MsgId::ServiceTxRequest,
        .type = TaskUsbMsgType,
      };
      os::msg::send_msg(os::msg::MsgQueue::UsbTaskQueue, &msg);
    }
  }
}

int32_t uartTask_postRequest(const uint8_t* data, size_t size) {
  return task::uart_service_.postRequest(data, size);
}

int32_t uartTask_getRequest(uint8_t* data, size_t max_size) {
  return task::uart_service_.serviceRequest(data, max_size);
}

}  // namespace task
