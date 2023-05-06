/*
 * uart_task.cpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#include "task/uart_task.hpp"

#include "cmsis_os.h"

namespace task {

void uartTask(void* argument) {
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
}

}  // namespace task
