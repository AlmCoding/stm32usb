/*
 * receive_msg.hpp
 *
 *  Created on: May 7, 2023
 *      Author: Alexander L.
 */

#ifndef OS_MSG_RECEIVE_MSG_HPP_
#define OS_MSG_RECEIVE_MSG_HPP_

#include "common.hpp"
#include "os/msg/msg_def.hpp"

namespace os {
namespace msg {

bool receive_msg(MsgQueue queue, MsgType* msg, TickNum timeout);

}  // namespace msg
}  // namespace os

#endif /* OS_MSG_RECEIVE_MSG_HPP_ */
