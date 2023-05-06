/*
 * queue_cfg.cpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#include "os/queue.hpp"
#include "os/msg/send_msg.hpp"

namespace os {

/* Definitions for ctrlTaskQueue */
static osMessageQueueId_t ctrlTaskQueueHandle;
static uint8_t ctrlTaskQueueBuffer[CtrlTaskQueueSize * sizeof(msg::MsgType)];
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
static uint8_t uartTaskQueueBuffer[UartTaskQueueSize * sizeof(msg::MsgType)];
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
  ctrlTaskQueueHandle = osMessageQueueNew(CtrlTaskQueueSize, sizeof(msg::MsgType), &ctrlTaskQueue_attributes);

  /* creation of uartTaskQueue */
  uartTaskQueueHandle = osMessageQueueNew(UartTaskQueueSize, sizeof(msg::MsgType), &uartTaskQueue_attributes);
}

osMessageQueueId_t getCtrlTaskQueue() {
  return uartTaskQueueHandle;
}

osMessageQueueId_t getUartTaskQueue() {
  return uartTaskQueueHandle;
}

}  // namespace os
