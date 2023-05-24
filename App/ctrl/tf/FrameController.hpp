/*
 * FrameController.hpp
 *
 *  Created on: 24 May 2023
 *      Author: Alexander L.
 */

#ifndef CTRL_TF_FRAMECONTROLLER_HPP_
#define CTRL_TF_FRAMECONTROLLER_HPP_

#ifdef __cplusplus

extern "C" {
#include "tf/TinyFrame.h"
}

namespace ctrl {
namespace tf {

class FrameController {
 public:
  // Deleted copy constructor and assignment operator to enforce singleton
  FrameController(const FrameController&) = delete;
  FrameController& operator=(const FrameController&) = delete;

  static FrameController& getInstance() {
    static FrameController instance;
    return instance;
  }

  void sendFrame();

  // Forward received data to tiny frame
  void receiveData(const uint8_t* data, size_t size);

 private:
  FrameController();

  TinyFrame tf_;
};

} /* namespace tf */
} /* namespace ctrl */
#endif

#ifdef __cplusplus
extern "C" {
#endif
void FrameController_receiveData(const uint8_t* data, size_t size);
#ifdef __cplusplus
}
#endif

#endif /* CTRL_TF_FRAMECONTROLLER_HPP_ */
