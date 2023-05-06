/*
 * queue_cfg.cpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#include "os/queue.hpp"

namespace os {

/* Definitions for ctrlTaskQueue */
static osMessageQueueId_t ctrlTaskQueueHandle;
static uint8_t ctrlTaskQueueBuffer[16 * sizeof(uint16_t)];
static osStaticMessageQDef_t ctrlTaskQueueControlBlock;
static const osMessageQueueAttr_t ctrlTaskQueue_attributes = { .name = "ctrlTaskQueue",
                                                               .cb_mem = &ctrlTaskQueueControlBlock,
                                                               .cb_size = sizeof(ctrlTaskQueueControlBlock),
                                                               .mq_mem = &ctrlTaskQueueBuffer,
                                                               .mq_size = sizeof(ctrlTaskQueueBuffer) };

void createQueues() {
  /* Create the queue(s) */
  /* creation of ctrlTaskQueue */
  ctrlTaskQueueHandle = osMessageQueueNew(16, sizeof(uint16_t), &ctrlTaskQueue_attributes);
}

}  // namespace os
