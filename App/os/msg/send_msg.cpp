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

void send_msg(MsgQueue queue, MsgType& msg) {
  osMessageQueueId_t qhdl = os::getQueue(queue);
  xQueueSend(qhdl, &msg, 0);
}

void send_msg_isr(MsgQueue queue, MsgType& msg) {
  osMessageQueueId_t qhdl = os::getQueue(queue);
  xQueueSendFromISR(qhdl, &msg, pdFALSE);
}

}  // namespace msg
}  // namespace os
