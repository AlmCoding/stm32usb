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
  static os::msg::MsgType msg;
  static bool msg_avail = false;
  static app::uart_srv::UartService uart_service{};

  /* Infinite loop */
  for (;;) {
    msg_avail = os::msg::receive_msg(os::msg::MsgQueue::UartTaskQueue, &msg, Ticks1ms);
    if (msg_avail == true) {
      // process msg
    }

    uart_service.run();
    // osDelay(1);
  }
}

}  // namespace task
