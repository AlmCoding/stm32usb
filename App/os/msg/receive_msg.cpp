/*
 * receive_msg.cpp
 *
 *  Created on: May 7, 2023
 *      Author: Alexander L.
 */

#include "os/msg/receive_msg.hpp"
#include "os/queue.hpp"
#include "queue.h"

namespace os {
namespace msg {

bool receive_msg(MsgQueue queue, MsgType* msg, TickNum timeout) {
  bool success;

  osMessageQueueId_t qhdl = os::getQueue(queue);
  success = static_cast<bool>(xQueueReceive(qhdl, msg, timeout));

  return success;
}

}  // namespace msg
}  // namespace os
