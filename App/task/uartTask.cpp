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

static app::uart_srv::UartService uart_service_{};

void uartTask(void* /*argument*/) {
  static os::msg::BaseMsg msg;

  uart_service_.init();

  // Register callback for incoming msg
  driver::tf::FrameDriver::getInstance().registerRxCallback(app::usb::UsbMsgType::UartMsg, uartTask_postRequest);

  // Register callback for outgoing msg
  driver::tf::FrameDriver::getInstance().registerTxCallback(app::usb::UsbMsgType::UartMsg, uartTask_getRequest);

  /* Infinite loop */
  for (;;) {
    if (os::msg::receive_msg(os::msg::MsgQueue::UartTaskQueue, &msg, os::CycleTime_UartTask) == true) {
      // process msg
    }

    uart_service_.run();
  }
}

int32_t uartTask_postRequest(const uint8_t* data, size_t size) {
  return task::uart_service_.postTxRequest(data, size);
}

int32_t uartTask_getRequest(uint8_t* data, size_t max_size) {
  return task::uart_service_.getRxRequest(data, max_size);
}

}  // namespace task
