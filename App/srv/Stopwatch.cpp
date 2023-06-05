/*
 * Stopwatch.cpp
 *
 *  Created on: May 31, 2023
 *      Author: Alexander L.
 */

#include "srv/Stopwatch.hpp"
#include "tim.h"

namespace srv {

constexpr MicroSeconds TimeBase = 1;

Stopwatch::Stopwatch() {}

Stopwatch::~Stopwatch() {}

void Stopwatch::start() {
  running_ = true;
  start_ = htim2.Instance->CNT;
}

MicroSeconds Stopwatch::time() {
  if (running_ == true) {
    return (htim2.Instance->CNT - start_) * TimeBase;
  } else {
    return time_;
  }
}

MicroSeconds Stopwatch::stop() {
  time_ = (htim2.Instance->CNT - start_) * TimeBase;
  running_ = false;
  return time_;
}

} /* namespace srv */
