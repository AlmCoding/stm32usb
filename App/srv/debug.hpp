/*
 * debug.hpp
 *
 *  Created on: 18 May 2023
 *      Author: Alexander L.
 */

#ifndef SRV_DEBUG_HPP_
#define SRV_DEBUG_HPP_

#include "common.hpp"

namespace srv {
namespace debug {

constexpr uint8_t TERM0 = 0;

void initDebug();

void print(uint8_t term, const char* format, ...);

}  // namespace debug
}  // namespace srv

#endif /* SRV_DEBUG_HPP_ */
