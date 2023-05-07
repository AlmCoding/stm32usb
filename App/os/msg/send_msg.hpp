/*
 * send_msg.hpp
 *
 *  Created on: May 6, 2023
 *      Author: Alexander L.
 */

#ifndef OS_MSG_SEND_MSG_HPP_
#define OS_MSG_SEND_MSG_HPP_

#include "os/msg/msg_def.hpp"
#include "os/queue.hpp"

namespace os {
namespace msg {

void send_msg(MsgQueue queue, MsgType& msg);
void send_msg_isr(MsgQueue queue, MsgType& msg);

}  // namespace msg
}  // namespace os

#endif /* OS_MSG_SEND_MSG_HPP_ */
