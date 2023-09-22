/*
 * msg_broker.hpp
 *
 *  Created on: May 8, 2023
 *      Author: Alexander L.
 */

#ifndef OS_MSG_MSG_BROKER_HPP_
#define OS_MSG_MSG_BROKER_HPP_

#include "common.hpp"
#include "os/msg/msg_def.hpp"

namespace os::msg {

bool send_msg(MsgQueue queue, BaseMsg* msg);
bool receive_msg(MsgQueue queue, BaseMsg* msg, TickNum_t timeout);

}  // namespace os::msg

#endif /* OS_MSG_MSG_BROKER_HPP_ */
