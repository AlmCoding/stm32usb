/*
 * usbTask.cpp
 *
 *  Created on: 15 May 2023
 *      Author: Alexander L.
 */

#include "task/usbTask.hpp"

#include "cmsis_os.h"
#include "driver/tf/FrameDriver.hpp"
#include "os/msg/msg_broker.hpp"
#include "os/task.hpp"
#include "srv/debug.hpp"
#include "task/uartTask.hpp"
#include "usbd_cdc_if.h"

#define DEBUG_ENABLE_USB_TASK
#ifdef DEBUG_ENABLE_USB_TASK
#define DEBUG_INFO(format, ...) srv::debug::print(srv::debug::TERM0, "[INF][usbTask]: " format "\n", ##__VA_ARGS__);
#define DEBUG_WARN(format, ...) srv::debug::print(srv::debug::TERM0, "[WRN][usbTask]: " format "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(format, ...) srv::debug::print(srv::debug::TERM0, "[ERR][usbTask]: " format "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

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
    case os::msg::MsgId::ServiceTxRequest: {
      driver::tf::FrameDriver::getInstance().callTxCallback(msg->type);
      break;
    }
    default: {
      break;
    }
  }
}

int32_t usbTask_transmitData(const uint8_t* /*data*/, size_t /*size*/) {
  return 0;
}

int32_t usbTask_receiveData(const uint8_t* /*data*/, size_t /*size*/) {
  return 0;
}

}  // namespace task
