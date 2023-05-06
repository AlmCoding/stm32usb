/*
 * uart_task.cpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#include "task/uart_task.hpp"

#include "app/uart_srv/UartService.hpp"
#include "cmsis_os.h"
#include "lib/etl/vector.h"

namespace task {

void uartTask(void* argument) {
  static app::uart_srv::UartService uart_service{};

  /* Infinite loop */
  for (;;) {
    uart_service.run();

    osDelay(1);
  }
}

}  // namespace task
