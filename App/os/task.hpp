/*
 * task_cfg.hpp
 *
 *  Created on: May 4, 2023
 *      Author: Alexander L.
 */

#ifndef OS_TASK_HPP_
#define OS_TASK_HPP_

#include "cmsis_os.h"
#include "task.h"

namespace os {

constexpr TickType_t CycleTime_CtrlTask = 1;
constexpr TickType_t CycleTime_UartTask = 1;
constexpr TickType_t CycleTime_GpioTask = 1;

constexpr size_t StackSize_CtrlTask = 4096;
constexpr size_t StackSize_UartTask = 2048;
constexpr size_t StackSize_GpioTask = 1024;

constexpr osPriority_t Priority_CtrlTask = osPriorityNormal;
constexpr osPriority_t Priority_UartTask = osPriorityNormal;
constexpr osPriority_t Priority_GpioTask = osPriorityNormal;

typedef StaticTask_t osStaticThreadDef_t;

void createTasks();

}  // namespace os

#endif /* OS_TASK_HPP_ */
