/*
 * Timeout.cpp
 *
 *  Created on: 24 Jun 2023
 *      Author: Alexander L.
 */

#include "srv/Timeout.hpp"
#include "tim.h"

namespace srv {

Timeout::Timeout() {}

Timeout::Timeout(MicroSeconds timeout) {
  timeout_ = timeout;
}

Timeout::~Timeout() {}

void Timeout::start(MicroSeconds timeout) {
  timeout_ = timeout;
  start_ = htim2.Instance->CNT;
}

bool Timeout::isExpired() {
  MicroSeconds time = (htim2.Instance->CNT - start_) * TimeBase;
  if (time >= timeout_) {
    return true;
  }
  return false;
}

MicroSeconds Timeout::remaining() {
  return timeout_ - ((htim2.Instance->CNT - start_) * TimeBase);
}

} /* namespace srv */
