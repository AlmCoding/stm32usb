/*
 * queue_cfg.cpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#include "os/queue.hpp"
#include "os/msg/msg_def.hpp"

namespace os {

/* Definitions for ctrlTaskQueue */
static osMessageQueueId_t ctrlTaskQueueHandle;
static uint8_t ctrlTaskQueueBuffer[QueueSize_CtrlTask * sizeof(msg::BaseMsg)];
static osStaticMessageQDef_t ctrlTaskQueueControlBlock;
static const osMessageQueueAttr_t ctrlTaskQueue_attributes = {
  .name = "ctrlTaskQueue",
  .cb_mem = &ctrlTaskQueueControlBlock,
  .cb_size = sizeof(ctrlTaskQueueControlBlock),
  .mq_mem = &ctrlTaskQueueBuffer,
  .mq_size = sizeof(ctrlTaskQueueBuffer),
};

/* Definitions for uartTaskQueue */
static osMessageQueueId_t uartTaskQueueHandle;
static uint8_t uartTaskQueueBuffer[QueueSize_UartTask * sizeof(msg::BaseMsg)];
static osStaticMessageQDef_t uartTaskQueueControlBlock;
static const osMessageQueueAttr_t uartTaskQueue_attributes = {
  .name = "uartTaskQueue",
  .cb_mem = &uartTaskQueueControlBlock,
  .cb_size = sizeof(uartTaskQueueControlBlock),
  .mq_mem = &uartTaskQueueBuffer,
  .mq_size = sizeof(uartTaskQueueBuffer),
};

void createQueues() {
  /* Create the queue(s) */

  /* creation of ctrlTaskQueue */
  ctrlTaskQueueHandle = osMessageQueueNew(QueueSize_CtrlTask, sizeof(msg::BaseMsg), &ctrlTaskQueue_attributes);

  /* creation of uartTaskQueue */
  uartTaskQueueHandle = osMessageQueueNew(QueueSize_UartTask, sizeof(msg::BaseMsg), &uartTaskQueue_attributes);
}

osMessageQueueId_t getQueue(msg::MsgQueue queue) {
  osMessageQueueId_t qhdl = nullptr;

  switch (queue) {
    case msg::MsgQueue::CtrlTaskQueue: {
      qhdl = ctrlTaskQueueHandle;
      break;
    }
    case msg::MsgQueue::UartTaskQueue: {
      qhdl = uartTaskQueueHandle;
      break;
    }
    default: {
      break;
    }
  }

  return qhdl;
}

}  // namespace os
