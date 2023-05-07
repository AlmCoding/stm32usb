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
  ServiceTxUart1 = 0,
};

typedef struct {
  MsgId id;
} MsgType;

}  // namespace msg
}  // namespace os

#endif /* OS_MSG_MSG_DEF_HPP_ */
