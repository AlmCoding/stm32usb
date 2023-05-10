/*
 * common.hpp
 *
 *  Created on: May 7, 2023
 *      Author: Alexander L.
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <cstdint>
#include <cstring>

typedef uint32_t TickNum;
typedef int32_t MilliSeconds;
typedef int32_t MicroSeconds;

constexpr TickNum Ticks1ms = 1;
constexpr TickNum Ticks5ms = 5 * Ticks1ms;
constexpr TickNum Ticks10ms = 10 * Ticks1ms;

enum class StatusType {
  Error = -1,
  Ok = 0,
  Busy = 1,
};

// TickType_t ticks = osKernelSysTickMicroSec(timeout);
#endif /* COMMON_HPP_ */
