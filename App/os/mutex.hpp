/*
 * mutex.hpp
 *
 *  Created on: Jun 22, 2023
 *      Author: Alexander L.
 */

#ifndef OS_MUTEX_HPP_
#define OS_MUTEX_HPP_

#include "cmsis_os.h"

namespace os {

typedef StaticSemaphore_t osStaticMutexDef_t;

extern osMutexId_t ServiceUartMutexHandle;
extern osMutexId_t ServiceGpioMutexHandle;

void createMutexes();

}  // namespace os

#endif /* OS_MUTEX_HPP_ */
