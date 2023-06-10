/*
 * ctrl_task.cpp
 *
 *  Created on: May 4, 2023
 *      Author: Alexander L.
 */

#include "task/ctrlTask.hpp"

#include "cmsis_os.h"
#include "driver/tf/FrameDriver.hpp"
#include "os/msg/msg_broker.hpp"
#include "os/task.hpp"
#include "srv/debug.hpp"
#include "task/uartTask.hpp"
#include "usbd_cdc_if.h"

#define DEBUG_ENABLE_CTRL_TASK
#ifdef DEBUG_ENABLE_CTRL_TASK
#define DEBUG_INFO(f, ...) srv::debug::print(srv::debug::TERM0, "[INF][ctrlTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::debug::print(srv::debug::TERM0, "[WRN][ctrlTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::debug::print(srv::debug::TERM0, "[ERR][ctrlTsk]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace task::ctrl {

void processMsg(os::msg::BaseMsg* msg);

void ctrlTask(void* /*argument*/) {
  static os::msg::BaseMsg msg;

  /* Infinite loop */
  for (;;) {
    if (os::msg::receive_msg(os::msg::MsgQueue::CtrlTaskQueue, &msg, os::CycleTime_CtrlTask) == true) {
      processMsg(&msg);
    }
  }
}

void processMsg(os::msg::BaseMsg* msg) {
  switch (msg->id) {
    case os::msg::MsgId::ServiceTxRequest: {
      DEBUG_INFO("Service tx request")
      while (CDC_IsTransmit_Busy() == 1) {
      }
      driver::tf::FrameDriver::getInstance().callTxCallback(msg->type);
      break;
    }
    default: {
      break;
    }
  }
}

}  // namespace task::ctrl
