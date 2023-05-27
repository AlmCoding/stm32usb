/*
 * usbComTypes.hpp
 *
 *  Created on: 27 May 2023
 *      Author: Alexander L.
 */

#ifndef APP_USB_USBCOMTYPES_HPP_
#define APP_USB_USBCOMTYPES_HPP_

#include "common.hpp"

extern "C" {
#include "tf/TF_Config.h"
}

namespace app::usb {

enum class UsbMsgType : uint8_t {
  CtrlMsg = 0,
  UartMsg,
  I2cMsg,
  // Keep this at end
  NumValues,
};

static_assert(static_cast<uint8_t>(UsbMsgType::NumValues) <= TF_MAX_TYPE_LST,
              "Increase TF_MAX_TYPE_LST in TF_Config.h file.");

typedef int32_t (*TxCallback)(uint8_t* data, size_t max_size);
typedef int32_t (*RxCallback)(const uint8_t* data, size_t size);

}  // namespace app::usb

#endif /* APP_USB_USBCOMTYPES_HPP_ */
