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
#include "task/uartTask.hpp"
#include "usbd_cdc_if.h"

namespace task {

constexpr size_t UsbTxBufferSize = 1024;
uint8_t usb_task_tx_buffer_[UsbTxBufferSize];

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
  size_t size = 0;

  switch (msg->id) {
    case os::msg::MsgId::UartTask2UsbTask_ServiceRxUart1: {
      size = task::uartTask_getRequest(usb_task_tx_buffer_, UsbTxBufferSize);
      break;
    }
    default: {
      break;
    }
  }

  if (size > 0) {
    CDC_Transmit_FS(usb_task_tx_buffer_, static_cast<uint16_t>(size));
  }
}

}  // namespace task
