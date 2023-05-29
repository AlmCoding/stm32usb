/*
 * debug.hpp
 *
 *  Created on: 18 May 2023
 *      Author: Alexander L.
 */

#ifndef SRV_DEBUG_HPP_
#define SRV_DEBUG_HPP_

#ifdef __cplusplus

#include "common.hpp"

namespace srv::debug {

constexpr uint8_t TERM0 = 0;

extern "C" {

#else
#define TERM0 0
#endif

void initDebug();
void print(uint8_t term, const char* format, ...);

#ifdef __cplusplus
}  // extern "C"
}  // namespace srv::debug
#endif

#endif /* SRV_DEBUG_HPP_ */
