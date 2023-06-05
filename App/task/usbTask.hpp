/*
 * usbTask.hpp
 *
 *  Created on: 15 May 2023
 *      Author: Alexander L.
 */

#ifndef TASK_USBTASK_HPP_
#define TASK_USBTASK_HPP_

#include "common.hpp"

namespace task {

void usbTask(void* argument);
int32_t usbTask_transmitData(uint8_t* data, size_t max_size);
int32_t usbTask_receiveData(const uint8_t* data, size_t size);

}  // namespace task

#endif /* TASK_USBTASK_HPP_ */
