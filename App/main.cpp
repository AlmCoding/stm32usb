/*
 * main.cpp
 *
 *  Created on: May 3, 2023
 *      Author: Alexander L.
 */

#include "main.h"
#include "os/builder.hpp"
#include "srv/debug.hpp"
#include "usb_device.h"

#define DEBUG_ENABLE_MAIN
#ifdef DEBUG_ENABLE_MAIN
#define DEBUG_INFO(f, ...) srv::debug::print(srv::debug::TERM0, "[INF][main]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::debug::print(srv::debug::TERM0, "[WRN][main]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::debug::print(srv::debug::TERM0, "[ERR][main]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  // Init debug output
  srv::debug::initDebug();

  // Init HW (CubeMX generated)
  initPeripherals();
  DEBUG_INFO("Init Peripherals [OK]");

  /* Init USB device */
  MX_USB_DEVICE_Init();
  DEBUG_INFO("Init USB [OK]");

  // Build os
  os::buildOs();
}
