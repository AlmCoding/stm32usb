/*
 * gpioTask.cpp
 *
 *  Created on: 1 Jul 2023
 *      Author: Alexander L.
 */

#include "task/gpioTask.hpp"

#include "app/gpio_srv/GpioService.hpp"
#include "cmsis_os.h"
#include "driver/tf/FrameDriver.hpp"
#include "os/msg/msg_broker.hpp"
#include "os/mutex.hpp"
#include "os/task.hpp"
#include "srv/debug.hpp"

#define DEBUG_ENABLE_GPIO_TASK
#ifdef DEBUG_ENABLE_GPIO_TASK
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][gpioTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][gpioTsk]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][gpioTsk]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace task::gpio {

int32_t gpioTask_postRequest(const uint8_t* data, size_t size);
int32_t gpioTask_serviceRequest(uint8_t* data, size_t max_size);

static app::gpio_srv::GpioService gpio_service_{};
static bool ongoing_service_ = false;
static uint32_t msg_count_ = 0;

void gpioTask(void* /*argument*/) {
  constexpr app::usb::UsbMsgType TaskUsbMsgType = app::usb::UsbMsgType::GpioMsg;
  static os::msg::BaseMsg msg;

  gpio_service_.init();

  // Register callback for incoming msg
  driver::tf::FrameDriver::getInstance().registerRxCallback(TaskUsbMsgType, gpioTask_postRequest);

  // Register callback for outgoing msg
  driver::tf::FrameDriver::getInstance().registerTxCallback(TaskUsbMsgType, gpioTask_serviceRequest);

  /* Infinite loop */
  for (;;) {
    if (os::msg::receive_msg(os::msg::MsgQueue::GpioTaskQueue, &msg, os::CycleTime_GpioTask) == true) {
      // process msg
    }

    if (osMutexAcquire(os::ServiceGpioMutexHandle, Ticks100ms) == osOK) {
      uint32_t service_requests = gpio_service_.poll();
      if ((service_requests > 0) && (ongoing_service_ == false)) {
        DEBUG_INFO("Request srv from ctrlTask")
        // Inform CtrlTask to service received data
        os::msg::BaseMsg req_msg = {
          .id = os::msg::MsgId::ServiceUpstreamRequest,
          .type = TaskUsbMsgType,
          .cnt = service_requests,
        };

        if (os::msg::send_msg(os::msg::MsgQueue::CtrlTaskQueue, &req_msg) == true) {
          ongoing_service_ = true;
          DEBUG_INFO("Send msg: %d [ok]", ++msg_count_)

        } else {
          DEBUG_ERROR("Send msg: %d [failed]", ++msg_count_)
        }

      } else if (service_requests > 0) {
        DEBUG_WARN("Wait srv cplt ...")
      }

      osMutexRelease(os::ServiceGpioMutexHandle);
    }
  }
}

int32_t gpioTask_postRequest(const uint8_t* data, size_t len) {
  return gpio_service_.postRequest(data, len);
}

int32_t gpioTask_serviceRequest(uint8_t* data, size_t max_len) {
  int32_t len = -1;

  if (osMutexAcquire(os::ServiceGpioMutexHandle, Ticks100ms) == osOK) {
    len = gpio_service_.serviceRequest(data, max_len);
    ongoing_service_ = false;

    DEBUG_INFO("Service request: %d [ok]", msg_count_)
    osMutexRelease(os::ServiceGpioMutexHandle);

  } else {
    DEBUG_ERROR("Service request: %d [failed]", msg_count_)
  }

  return len;
}

}  // namespace task::gpio
