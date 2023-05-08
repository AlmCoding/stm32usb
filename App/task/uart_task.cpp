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

namespace task {

void uartTask(void* /*argument*/) {
  static os::msg::BaseMsg msg;
  static app::uart_srv::UartService uart_service{};

  /* Infinite loop */
  for (;;) {
    if (os::msg::receive_msg(os::msg::MsgQueue::UartTaskQueue, &msg, Ticks1ms) == true) {
      // process msg
    }

    uart_service.run();
    // osDelay(1);
  }
}

}  // namespace task
