/*
 * Timeout.hpp
 *
 *  Created on: 24 Jun 2023
 *      Author: Alexander L.
 */

#ifndef SRV_TIMEOUT_HPP_
#define SRV_TIMEOUT_HPP_

#include "common.hpp"

namespace srv {

class Timeout {
 public:
  Timeout();
  Timeout(MicroSeconds timeout);
  virtual ~Timeout();

  void start(MicroSeconds timeout);
  bool isExpired();
  MicroSeconds remaining();

 private:
  MicroSeconds timeout_;
  MicroSeconds start_;
};

} /* namespace srv */

#endif /* SRV_TIMEOUT_HPP_ */
