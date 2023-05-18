/*
 * main.cpp
 *
 *  Created on: May 3, 2023
 *      Author: Alexander L.
 */

#include "main.h"
#include "os/builder.hpp"
#include "rtt/RTT/SEGGER_RTT.h"
#include "usb_device.h"

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  SEGGER_RTT_Init();

  // Init HW (CubeMX generated)
  initPeripherals();

  /* Init USB device */
  MX_USB_DEVICE_Init();

  // Build os
  os::buildOs();
}
