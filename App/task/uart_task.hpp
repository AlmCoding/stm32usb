/*
 * uart1_task.hpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#ifndef TASK_UART_TASK_HPP_
#define TASK_UART_TASK_HPP_

#ifdef __cplusplus
#include "common.hpp"
namespace task {
void uartTask(void* argument);
}  // namespace task
#endif

#ifdef __cplusplus
extern "C" {
#else
#include <stddef.h>
#endif

int32_t handleRequest(const uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* TASK_UART_TASK_HPP_ */
