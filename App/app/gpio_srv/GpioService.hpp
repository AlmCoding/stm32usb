/*
 * GpioService.hpp
 *
 *  Created on: 1 Jul 2023
 *      Author: Alexander L.
 */

#ifndef APP_GPIO_SRV_GPIOSERVICE_HPP_
#define APP_GPIO_SRV_GPIOSERVICE_HPP_

#include "gpio.h"
#include "hal/gpio/Gpio.hpp"
#include "proto_c/uart.pb.h"

namespace app::gpio_srv {

class GpioService {
 public:
  GpioService();
  virtual ~GpioService();

  void init();
  uint32_t poll();

  int32_t postRequest(const uint8_t* data, size_t len);
  int32_t serviceRequest(uint8_t* data, size_t max_len);

 private:
  hal::gpio::Gpio gpio1_{ GPIOC, GPIO_0_Pin, hal::gpio::GpioId::Gpio1 };
  hal::gpio::Gpio gpio2_{ GPIOC, GPIO_1_Pin, hal::gpio::GpioId::Gpio2 };
  hal::gpio::Gpio gpio3_{ GPIOC, GPIO_2_Pin, hal::gpio::GpioId::Gpio3 };
  hal::gpio::Gpio gpio4_{ GPIOC, GPIO_3_Pin, hal::gpio::GpioId::Gpio4 };

  uint32_t seqence_number_ = 0;
  bool send_data_msg_ = false;
};

}  // namespace app::gpio_srv

#endif /* APP_GPIO_SRV_GPIOSERVICE_HPP_ */
