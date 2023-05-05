/*
 * task_cfg.hpp
 *
 *  Created on: May 4, 2023
 *      Author: Alexander L.
 */

#ifndef OS_TASK_HPP_
#define OS_TASK_HPP_

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

namespace os {

typedef StaticTask_t osStaticThreadDef_t;

void create_tasks();

} // namespace os

#endif /* OS_TASK_HPP_ */
