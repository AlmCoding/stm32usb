/*
 * task_builder.cpp
 *
 *  Created on: May 4, 2023
 *      Author: Alexander L.
 */

#include "os/builder.hpp"

#include "cmsis_os.h"
#include "os/timer.hpp"
#include "os/queue.hpp"
#include "os/task.hpp"

namespace os {

void build_os() {
  /* Init scheduler */
  osKernelInitialize();

  create_timers();
  create_queues();
  create_tasks();

  /* Start scheduler */
  osKernelStart();

  /* We never get here as control is taken by the scheduler */
  /* Infinite loop */
  while(1);
}

} // namespace os
