/*
 * task_cfg.cpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#include "os/task.hpp"

#include "task/ctrlTask.hpp"
#include "task/uartTask.hpp"

namespace os {

/* Definitions for ctrlTask */
static osThreadId_t ctrlTaskHandle;
static uint8_t ctrlTaskBuffer[StackSize_CtrlTask];
static osStaticThreadDef_t ctrlTaskControlBlock;
static const osThreadAttr_t ctrlTask_attributes = {
  .name = "ctrlTask",
  .cb_mem = &ctrlTaskControlBlock,
  .cb_size = sizeof(ctrlTaskControlBlock),
  .stack_mem = &ctrlTaskBuffer[0],
  .stack_size = sizeof(ctrlTaskBuffer),
  .priority = Priority_CtrlTask,
};

/* Definitions for uartTask */
static osThreadId_t uartTaskHandle;
static uint8_t uartTaskBuffer[StackSize_UartTask];
static osStaticThreadDef_t uartTaskControlBlock;
static const osThreadAttr_t uartTask_attributes = {
  .name = "uartTask",
  .cb_mem = &uartTaskControlBlock,
  .cb_size = sizeof(uartTaskControlBlock),
  .stack_mem = &uartTaskBuffer[0],
  .stack_size = sizeof(uartTaskBuffer),
  .priority = Priority_UartTask,
};

void createTasks() {
  /* Create the thread(s) */

  /* creation of ctrlTask */
  ctrlTaskHandle = osThreadNew(task::ctrl::ctrlTask, NULL, &ctrlTask_attributes);

  /* creation of uartTask */
  uartTaskHandle = osThreadNew(task::uart::uartTask, NULL, &uartTask_attributes);
}

}  // namespace os
