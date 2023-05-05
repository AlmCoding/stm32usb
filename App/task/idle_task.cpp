/*
 * idle_task.cpp
 *
 *  Created on: May 4, 2023
 *      Author: Alexander L.
 */

#include "task/idle_task.hpp"

#include "cmsis_os.h"

namespace task {

void idle_task(void *argument) {
  /* Infinite loop */
  for(;;)
  {
	osDelay(1);
  }
}

} // namespace task
