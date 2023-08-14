/*
 * i2cTask.cpp
 *
 *  Created on: Aug 14, 2023
 *      Author: Alexander L.
 */

#include "task/i2cTask.hpp"

#include "cmsis_os.h"
#include "os/msg/msg_broker.hpp"
#include "os/task.hpp"
#include "srv/Stopwatch.hpp"
#include "srv/debug.hpp"

#define DEBUG_ENABLE_I2C_TASK
#ifdef DEBUG_ENABLE_I2C_TASK
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][i2cTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][i2cTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][i2cTsk]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace task::i2c {

static uint32_t msg_count_ = 0;

void processMsg(os::msg::BaseMsg* msg);

void i2cTask(void* /*argument*/) {
  os::msg::BaseMsg msg;

  /* Infinite loop */
  for (;;) {
    if (os::msg::receive_msg(os::msg::MsgQueue::I2cTaskQueue, &msg, os::CycleTime_I2cTask) == true) {
      processMsg(&msg);
    }
  }
}

void processMsg(os::msg::BaseMsg* msg) {
  srv::Stopwatch stopwatch{};
  DEBUG_INFO("Notification received: %d", ++msg_count_)

  switch (msg->id) {
    case os::msg::MsgId::ServiceUpstreamRequest: {
      break;
    }
    default: {
      break;
    }
  }
}

}  // namespace task::i2c
