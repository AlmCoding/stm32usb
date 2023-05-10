/*
 * msg_def.hpp
 *
 *  Created on: May 7, 2023
 *      Author: Alexander L.
 */

#ifndef OS_MSG_MSG_DEF_HPP_
#define OS_MSG_MSG_DEF_HPP_

namespace os {
namespace msg {

enum class MsgQueue {
  CtrlTaskQueue = 0,
  UartTaskQueue,
};

enum class MsgId {
  ServiceTxUart1 = 0,   // From usb int to uart task
  ServiceRxUart1,		// From uart task to usb task
};

typedef struct {
  MsgId id;
} BaseMsg;

}  // namespace msg
}  // namespace os

#endif /* OS_MSG_MSG_DEF_HPP_ */
