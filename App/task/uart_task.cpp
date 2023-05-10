/*
 * uart_task.cpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#include "task/uart_task.hpp"

#include "app/uart_srv/UartService.hpp"
#include "cmsis_os.h"
#include "lib/etl/vector.h"
#include "os/msg/msg_broker.hpp"
#include "os/task.hpp"

namespace task {

static app::uart_srv::UartService uart_service_{};

void uartTask(void* /*argument*/) {
  static os::msg::BaseMsg msg;

  /* Infinite loop */
  for (;;) {
    if (os::msg::receive_msg(os::msg::MsgQueue::UartTaskQueue, &msg, os::CycleTime_UartTask) == true) {
      // process msg
    }

    uart_service_.run();
  }
}

}  // namespace task

int32_t handleRequest(const uint8_t* data, size_t size) {
  return task::uart_service_.forwardTxRequest(data, size);
}
