/*
 * mutex.cpp
 *
 *  Created on: Jun 22, 2023
 *      Author: Alexander L.
 */

#include "os/mutex.hpp"

namespace os {

/* Definitions for ServiceUartMutex */
osMutexId_t ServiceUartMutexHandle;
osStaticMutexDef_t ServiceUartMutexControlBlock;
const osMutexAttr_t ServiceUartMutex_attributes = {
  .name = "ServiceUartMutex",
  .cb_mem = &ServiceUartMutexControlBlock,
  .cb_size = sizeof(ServiceUartMutexControlBlock),
};

/* Definitions for ServiceGpioMutex */
osMutexId_t ServiceGpioMutexHandle;
osStaticMutexDef_t ServiceGpioMutexControlBlock;
const osMutexAttr_t ServiceGpioMutex_attributes = {
  .name = "ServiceGpioMutex",
  .cb_mem = &ServiceGpioMutexControlBlock,
  .cb_size = sizeof(ServiceGpioMutexControlBlock),
};

void createMutexes() {
  /* Create the mutex(es) */

  /* creation of ServiceUartMutex */
  ServiceUartMutexHandle = osMutexNew(&ServiceUartMutex_attributes);

  /* creation of ServiceGpioMutex */
  ServiceGpioMutexHandle = osMutexNew(&ServiceGpioMutex_attributes);
}

}  // namespace os
