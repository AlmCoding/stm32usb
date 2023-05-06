/*
 * send_msg.cpp
 *
 *  Created on: May 6, 2023
 *      Author: Alexander L.
 */

#include "os/msg/send_msg.hpp"
#include "cmsis_os.h"
#include "queue.h"

namespace os {
namespace msg {

osMessageQueueId_t getQueue(MsgQueue queue);

void send_msg(MsgQueue queue, MsgType& msg) {
  xQueueSend(getQueue(queue), &msg, 0);
}

void send_msg_isr(MsgQueue queue, MsgType& msg) {
  xQueueSendFromISR(getQueue(queue), &msg, pdFALSE);
}

osMessageQueueId_t getQueue(MsgQueue queue) {
  osMessageQueueId_t qhdl;

  if (queue == MsgQueue::CtrlTaskQueue) {
    qhdl = os::getUartTaskQueue();
  } else {
    qhdl = os::getUartTaskQueue();
  }

  return qhdl;
}

}  // namespace msg
}  // namespace os
