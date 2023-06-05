/*
 * uart1_task.hpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#ifndef TASK_UART_TASK_HPP_
#define TASK_UART_TASK_HPP_

#include "common.hpp"

namespace task {

void uartTask(void* argument);
int32_t uartTask_postRequest(const uint8_t* data, size_t size);
int32_t uartTask_getRequest(uint8_t* data, size_t max_size);

}  // namespace task

#endif /* TASK_UART_TASK_HPP_ */
