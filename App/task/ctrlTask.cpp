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
#include "srv/Stopwatch.hpp"
#include "srv/debug.hpp"
#include "task/uartTask.hpp"
#include "usbd_cdc_if.h"

#define DEBUG_ENABLE_CTRL_TASK
#ifdef DEBUG_ENABLE_CTRL_TASK
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][ctrlTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][ctrlTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][ctrlTsk]: " f "\n", ##__VA_ARGS__);
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
  srv::Stopwatch stopwatch{};

  switch (msg->id) {
    case os::msg::MsgId::ServiceTxRequest: {
      while (msg->cnt > 0) {
        stopwatch.start();
        driver::tf::FrameDriver::getInstance().callTxCallback(msg->type);
        stopwatch.stop();
        DEBUG_INFO("USB tx (%d): %d us", msg->cnt, stopwatch.time());
        msg->cnt--;
      }
      break;
    }
    default: {
      break;
    }
  }
}

}  // namespace task::ctrl
