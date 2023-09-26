/*
 * i2cTask.cpp
 *
 *  Created on: Aug 14, 2023
 *      Author: Alexander L.
 */

#include "task/i2cTask.hpp"
#include "util/debug.hpp"

#include "app/i2c_srv/I2cService.hpp"
#include "cmsis_os.h"
#include "driver/tf/FrameDriver.hpp"
#include "os/msg/msg_broker.hpp"
#include "os/mutex.hpp"
#include "os/task.hpp"

#define DEBUG_ENABLE_I2C_TASK
#ifdef DEBUG_ENABLE_I2C_TASK
#define DEBUG_INFO(f, ...) util::dbg::print(util::dbg::TERM0, "[INF][i2cTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) util::dbg::print(util::dbg::TERM0, "[WRN][i2cTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) util::dbg::print(util::dbg::TERM0, "[ERR][i2cTsk]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace task::i2c {

void i2cTask_requestService_cb(os::msg::RequestCnt cnt);
int32_t i2cTask_postRequest_cb(const uint8_t* data, size_t size);
int32_t i2cTask_serviceRequest_cb(uint8_t* data, size_t max_size);

constexpr app::usb::UsbMsgType TaskUsbMsgType = app::usb::UsbMsgType::I2cMsg;
static app::i2c_srv::I2cService i2c_service_{};
static bool ongoing_service_ = false;
static uint32_t msg_count_ = 0;

void i2cTask(void* /*argument*/) {
  os::msg::BaseMsg msg;

  // Initialize service with notification callback
  i2c_service_.init(i2cTask_requestService_cb);

  // Register callback for incoming msg
  driver::tf::FrameDriver::getInstance().registerRxCallback(TaskUsbMsgType, i2cTask_postRequest_cb);

  // Register callback for outgoing msg
  driver::tf::FrameDriver::getInstance().registerTxCallback(TaskUsbMsgType, i2cTask_serviceRequest_cb);

  /* Infinite loop */
  for (;;) {
    if (os::msg::receive_msg(os::msg::MsgQueue::I2cTaskQueue, &msg, os::CycleTime_I2cTask) == true) {
      // process msg
    }

    i2c_service_.poll();
  }
}

void i2cTask_requestService_cb(os::msg::RequestCnt cnt) {
  if (ongoing_service_ == true) {
    return;
  }
  ongoing_service_ = true;

  os::msg::BaseMsg req_msg = {
    .id = os::msg::MsgId::ServiceUpstreamRequest,
    .type = TaskUsbMsgType,
    .cnt = cnt,
  };

  if (os::msg::send_msg(os::msg::MsgQueue::CtrlTaskQueue, &req_msg) == true) {
    DEBUG_INFO("Notify ctrlTask: %d [ok]", ++msg_count_)
  } else {
    DEBUG_ERROR("Notify ctrlTask: %d [failed]", ++msg_count_)
  }
}

int32_t i2cTask_postRequest_cb(const uint8_t* data, size_t len) {
  return i2c_service_.postRequest(data, len);
}

int32_t i2cTask_serviceRequest_cb(uint8_t* data, size_t max_len) {
  ongoing_service_ = false;
  int32_t len = i2c_service_.serviceRequest(data, max_len);

  DEBUG_INFO("Service request: %d [ok]", msg_count_)
  return len;
}

}  // namespace task::i2c
