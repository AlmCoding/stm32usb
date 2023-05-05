/*
 * queue_cfg.hpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#ifndef OS_QUEUE_HPP_
#define OS_QUEUE_HPP_

#include "cmsis_os.h"

namespace os {

typedef StaticQueue_t osStaticMessageQDef_t;

void create_queues();

}  // namespace os

#endif /* OS_QUEUE_HPP_ */
