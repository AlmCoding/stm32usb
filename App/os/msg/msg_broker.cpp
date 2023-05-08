/*
 * msg_broker.cpp
 *
 *  Created on: May 8, 2023
 *      Author: Alexander L.
 */

#include "os/msg/msg_broker.hpp"
#include "cmsis_os.h"
#include "os/queue.hpp"

namespace os {
namespace msg {

bool send_msg(MsgQueue queue, BaseMsg* msg) {
  bool success = false;

  osMessageQueueId_t qhdl = os::getQueue(queue);
  if (osMessageQueuePut(qhdl, msg, 0, 0) == osOK) {
    success = true;
  }

  return success;
}

bool receive_msg(MsgQueue queue, BaseMsg* msg, TickNum timeout) {
  bool success = false;

  osMessageQueueId_t qhdl = os::getQueue(queue);
  if (osMessageQueueGet(qhdl, msg, 0, timeout) == osOK) {
    success = true;
  }

  return success;
}

}  // namespace msg
}  // namespace os
