/*
 * queue_cfg.hpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#ifndef OS_QUEUE_HPP_
#define OS_QUEUE_HPP_

#include "cmsis_os.h"
#include "os/msg/msg_def.hpp"

namespace os {

typedef StaticQueue_t osStaticMessageQDef_t;

constexpr size_t QueueSize_CtrlTask = 8;
constexpr size_t QueueSize_UartTask = 8;

void createQueues();
osMessageQueueId_t getQueue(msg::MsgQueue queue);

}  // namespace os

#endif /* OS_QUEUE_HPP_ */
