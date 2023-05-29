/*
 * debug.cpp
 *
 *  Created on: 18 May 2023
 *      Author: Alexander L.
 */

#include "srv/debug.hpp"
#include <cstdarg>
#include "rtt/RTT/SEGGER_RTT.h"

namespace srv::debug {

void initDebug() {
  SEGGER_RTT_Init();
}

void print(uint8_t term, const char* format, ...) {
  va_list args;
  va_start(args, format);
  SEGGER_RTT_vprintf(term, format, &args);
  va_end(args);
}

}  // namespace srv::debug
