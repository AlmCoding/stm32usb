/*
 * FrameController.cpp
 *
 *  Created on: 24 May 2023
 *      Author: Alexander L.
 */

#include "ctrl/tf/FrameController.hpp"

namespace ctrl {
namespace tf {

FrameController::FrameController() {
  TF_InitStatic(&tf_, TF_SLAVE);
}

void FrameController::receiveData(const uint8_t* data, size_t size) {
  TF_Accept(&tf_, data, size);
}

} /* namespace tf */
} /* namespace ctrl */

void FrameController_receiveData(const uint8_t* data, size_t size) {
  ctrl::tf::FrameController::getInstance().receiveData(data, size);
}
