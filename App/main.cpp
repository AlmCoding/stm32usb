/*
 * main.cpp
 *
 *  Created on: May 3, 2023
 *      Author: Alexander L.
 */

#include "main.h"
#include "os/builder.hpp"
#include "usb_device.h"

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  // Init HW (CubeMX generated)
  init_peripherals();

  /* Init USB device */
  MX_USB_DEVICE_Init();

  // Build os
  os::build_os();
}
