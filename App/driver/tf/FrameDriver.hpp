/*
 * FrameDriver.hpp
 *
 *  Created on: 24 May 2023
 *      Author: Alexander L.
 */

#ifndef DRIVER_TF_FRAMEDRIVER_HPP_
#define DRIVER_TF_FRAMEDRIVER_HPP_

#ifdef __cplusplus

extern "C" {
#include "tf/TinyFrame.h"
}

namespace driver {
namespace tf {

class FrameDriver {
 public:
  // Deleted copy constructor and assignment operator to enforce singleton
  FrameDriver(const FrameDriver&) = delete;
  FrameDriver& operator=(const FrameDriver&) = delete;

  static FrameDriver& getInstance() {
    static FrameDriver instance;
    return instance;
  }

  void sendFrame();

  // Forward received data to tiny frame
  void receiveData(const uint8_t* data, size_t size);

 private:
  FrameDriver();

  TinyFrame tf_;
};

extern "C" {
#endif

void FrameDriver_receiveData(const uint8_t* data, size_t size);

#ifdef __cplusplus
}  // extern "C"
}  // namespace tf
}  // namespace driver
#endif

#endif /* DRIVER_TF_FRAMEDRIVER_HPP_ */
