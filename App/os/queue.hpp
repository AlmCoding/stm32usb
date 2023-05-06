/*
 * queue_cfg.hpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#ifndef OS_QUEUE_HPP_
#define OS_QUEUE_HPP_

#include "cmsis_os.h"

namespace os {

typedef StaticQueue_t osStaticMessageQDef_t;

constexpr size_t CtrlTaskQueueSize = 8;
constexpr size_t UartTaskQueueSize = 8;

void createQueues();
osMessageQueueId_t getUartTaskQueue();
osMessageQueueId_t getCtrlTaskQueue();

}  // namespace os

#endif /* OS_QUEUE_HPP_ */
