/*
 * task_cfg.cpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#include "os/task.hpp"

#include "task/ctrl_task.hpp"
#include "task/idle_task.hpp"
#include "task/uart_task.hpp"

namespace os {

/* Definitions for idleTask */
static osThreadId_t idleTaskHandle;
static uint32_t idleTaskBuffer[128];
static osStaticThreadDef_t idleTaskControlBlock;
static const osThreadAttr_t idleTask_attributes = {
  .name = "idleTask",
  .cb_mem = &idleTaskControlBlock,
  .cb_size = sizeof(idleTaskControlBlock),
  .stack_mem = &idleTaskBuffer[0],
  .stack_size = sizeof(idleTaskBuffer),
  .priority = (osPriority_t)osPriorityLow,
};

/* Definitions for ctrlTask */
static osThreadId_t ctrlTaskHandle;
static uint32_t ctrlTaskBuffer[256];
static osStaticThreadDef_t ctrlTaskControlBlock;
static const osThreadAttr_t ctrlTask_attributes = {
  .name = "ctrlTask",
  .cb_mem = &ctrlTaskControlBlock,
  .cb_size = sizeof(ctrlTaskControlBlock),
  .stack_mem = &ctrlTaskBuffer[0],
  .stack_size = sizeof(ctrlTaskBuffer),
  .priority = (osPriority_t)osPriorityNormal,
};

/* Definitions for uartTask */
static osThreadId_t uartTaskHandle;
static uint32_t uartTaskBuffer[256];
static osStaticThreadDef_t uartTaskControlBlock;
static const osThreadAttr_t uartTask_attributes = {
  .name = "uartTask",
  .cb_mem = &uartTaskControlBlock,
  .cb_size = sizeof(uartTaskControlBlock),
  .stack_mem = &uartTaskBuffer[0],
  .stack_size = sizeof(uartTaskBuffer),
  .priority = (osPriority_t)osPriorityNormal,
};

void createTasks() {
  /* Create the thread(s) */
  /* creation of idleTask */
  idleTaskHandle = osThreadNew(task::idleTask, NULL, &idleTask_attributes);

  /* creation of ctrlTask */
  ctrlTaskHandle = osThreadNew(task::ctrlTask, NULL, &ctrlTask_attributes);

  /* creation of uartTask */
  uartTaskHandle = osThreadNew(task::uartTask, NULL, &uartTask_attributes);
}

}  // namespace os
