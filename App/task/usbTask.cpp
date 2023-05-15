/*
 * usbTask.cpp
 *
 *  Created on: 15 May 2023
 *      Author: Alexander L.
 */

#include "task/usbTask.hpp"

#include "cmsis_os.h"
#include "os/msg/msg_broker.hpp"
#include "os/task.hpp"

namespace task {

void processMsg(os::msg::BaseMsg* msg);

void usbTask(void* /*argument*/) {
  static os::msg::BaseMsg msg;

  /* Infinite loop */
  for (;;) {
    if (os::msg::receive_msg(os::msg::MsgQueue::UsbTaskQueue, &msg, os::CycleTime_UsbTask) == true) {
      processMsg(&msg);
    }
  }
}

void processMsg(os::msg::BaseMsg* msg) {
  switch (msg->id) {
    case os::msg::MsgId::UartTask2UsbTask_ServiceRxUart1: {
      break;
    }
    default: {
      break;
    }
  }
}

}  // namespace task
