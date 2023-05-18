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
#define DEBUG_INFO(format, ...) srv::debug::print(srv::debug::TERM0, "[INF][main]: " format "\n", ##__VA_ARGS__);
#define DEBUG_WARN(format, ...) srv::debug::print(srv::debug::TERM0, "[WRN][main]: " format "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(format, ...) srv::debug::print(srv::debug::TERM0, "[ERR][main]: " format "\n", ##__VA_ARGS__);
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
  DEBUG_INFO("Entry Point");

  // Init HW (CubeMX generated)
  initPeripherals();

  /* Init USB device */
  MX_USB_DEVICE_Init();

  // Build os
  os::buildOs();
}
