/*
 * UartService.cpp
 *
 *  Created on: May 6, 2023
 *      Author: Alexander L.
 */

#include "app/uart_srv/UartService.hpp"
#include "usart.h"

namespace app {

namespace uart_srv {

UartService::UartService() : uart1{ &huart1 } {}

UartService::~UartService() {}

void UartService::run() {}

} /* namespace uart_srv */
} /* namespace app */
