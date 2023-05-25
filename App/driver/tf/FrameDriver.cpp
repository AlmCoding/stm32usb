/*
 * FrameDriver.cpp
 *
 *  Created on: 24 May 2023
 *      Author: Alexander L.
 */

#include "driver/tf/FrameDriver.hpp"

namespace driver {
namespace tf {

FrameDriver::FrameDriver() {
  TF_InitStatic(&tf_, TF_SLAVE);
}

void FrameDriver::receiveData(const uint8_t* data, size_t size) {
  TF_Accept(&tf_, data, size);
}

void FrameDriver_receiveData(const uint8_t* data, size_t size) {
  driver::tf::FrameDriver::getInstance().receiveData(data, size);
}

}  // namespace tf
}  // namespace driver
