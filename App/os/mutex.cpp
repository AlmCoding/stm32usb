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

void createMutexes() {
  /* Create the mutex(es) */

  /* creation of ServiceUartMutex */
  ServiceUartMutexHandle = osMutexNew(&ServiceUartMutex_attributes);
}

}  // namespace os
