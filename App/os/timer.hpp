/*
 * timer_cfg.hpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#ifndef OS_TIMER_HPP_
#define OS_TIMER_HPP_

#include "cmsis_os.h"

namespace os {

typedef StaticTimer_t osStaticTimerDef_t;

void createTimers();

}  // namespace os

#endif /* OS_TIMER_HPP_ */
