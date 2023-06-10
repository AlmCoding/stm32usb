/*
 * task_cfg.cpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#include "os/task.hpp"

#include "task/ctrlTask.hpp"
#include "task/uartTask.hpp"
#include "task/usbTask.hpp"

namespace os {

/* Definitions for usbTask */
static osThreadId_t usbTaskHandle;
static uint8_t usbTaskBuffer[StackSize_UsbTask];
static osStaticThreadDef_t usbTaskControlBlock;
static const osThreadAttr_t usbTask_attributes = {
  .name = "usbTask",
  .cb_mem = &usbTaskControlBlock,
  .cb_size = sizeof(usbTaskControlBlock),
  .stack_mem = &usbTaskBuffer[0],
  .stack_size = sizeof(usbTaskBuffer),
  .priority = Priority_UsbTask,
};

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
  /* creation of usbTask */
  usbTaskHandle = osThreadNew(task::usbTask, NULL, &usbTask_attributes);

  /* creation of ctrlTask */
  ctrlTaskHandle = osThreadNew(task::ctrlTask, NULL, &ctrlTask_attributes);

  /* creation of uartTask */
  uartTaskHandle = osThreadNew(task::uartTask, NULL, &uartTask_attributes);
}

}  // namespace os
