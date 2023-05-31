/*
 * Stopwatch.hpp
 *
 *  Created on: May 31, 2023
 *      Author: Alexander L.
 */

#ifndef SRV_STOPWATCH_HPP_
#define SRV_STOPWATCH_HPP_

#include "common.hpp"

namespace srv {

class Stopwatch {
 public:
  Stopwatch();
  virtual ~Stopwatch();

  void start();
  MicroSeconds time();
  MicroSeconds stop();

 private:
  bool running_ = false;
  MicroSeconds start_;
  MicroSeconds time_;
};

} /* namespace srv */

#endif /* SRV_STOPWATCH_HPP_ */
