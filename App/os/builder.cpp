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

namespace os {

void buildOs() {
  /* Init scheduler */
  osKernelInitialize();

  createTimers();
  createQueues();
  createTasks();

  /* Start scheduler */
  osKernelStart();

  /* We never get here as control is taken by the scheduler */
  /* Infinite loop */
  while (1) {
  }
}

}  // namespace os
