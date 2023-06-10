/*
 * task_builder.cpp
 *
 *  Created on: May 4, 2023
 *      Author: Alexander L.
 */

#include "os/builder.hpp"

#include "cmsis_os.h"
#include "os/queue.hpp"
#include "os/task.hpp"
#include "os/timer.hpp"

#define DEBUG_ENABLE_BUILDER
#ifdef DEBUG_ENABLE_BUILDER
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][builder]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][builder]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][builder]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace os {

void buildOs() {
  /* Init scheduler */
  osKernelInitialize();
  DEBUG_INFO("Init Kernel [OK]");

  createTimers();
  createQueues();
  createTasks();

  /* Start scheduler */
  DEBUG_INFO("Start Kernel ...");
  osKernelStart();

  /* We never get here as control is taken by the scheduler */
  /* Infinite loop */
  while (1) {
  }
}

}  // namespace os
