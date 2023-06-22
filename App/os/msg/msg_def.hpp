/*
 * msg_def.hpp
 *
 *  Created on: May 7, 2023
 *      Author: Alexander L.
 */

#ifndef OS_MSG_MSG_DEF_HPP_
#define OS_MSG_MSG_DEF_HPP_

#include "app/usb/usbComTypes.hpp"

namespace os::msg {

enum class MsgQueue {
  UsbTaskQueue = 0,
  CtrlTaskQueue,
  UartTaskQueue,
};

enum class MsgId {
  TriggerTask = 0,
  ServiceTxRequest,
};

typedef int32_t RequestCnt;

typedef struct {
  MsgId id;
  app::usb::UsbMsgType type;
  RequestCnt cnt;
} BaseMsg;

}  // namespace os::msg

#endif /* OS_MSG_MSG_DEF_HPP_ */
