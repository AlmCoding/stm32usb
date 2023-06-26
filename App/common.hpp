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

#define BITS_SET(reg, bits) ((bool)((reg & bits) == bits))
#define BITS_NOT_SET(reg, bits) (!BITS_SET(reg, bits))

typedef uint32_t TickNum_t;
typedef int32_t MilliSeconds;
typedef int32_t MicroSeconds;

constexpr MicroSeconds Time1ms = 1000;
constexpr MicroSeconds Time3ms = 3 * Time1ms;
constexpr MicroSeconds Time4ms = 5 * Time1ms;
constexpr MicroSeconds Time10ms = 10 * Time1ms;
constexpr MicroSeconds Time100ms = 100 * Time1ms;

constexpr TickNum_t Ticks1ms = 1;
constexpr TickNum_t Ticks3ms = 3 * Ticks1ms;
constexpr TickNum_t Ticks5ms = 5 * Ticks1ms;
constexpr TickNum_t Ticks10ms = 10 * Ticks1ms;
constexpr TickNum_t Ticks100ms = 100 * Ticks1ms;

enum class Status_t {
  Error = -1,
  Ok = 0,
  Busy = 1,
};

// TickType_t ticks = osKernelSysTickMicroSec(timeout);
#endif /* COMMON_HPP_ */
