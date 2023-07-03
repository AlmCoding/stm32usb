/*
 * Gpio.hpp
 *
 *  Created on: May 6, 2023
 *      Author: Alexander L.
 */

#ifndef HAL_GPIO_GPIO_HPP_
#define HAL_GPIO_GPIO_HPP_

#include "common.hpp"
#include "main.h"

namespace hal::gpio {

enum class GpioId {
  Gpio1 = 0,
  Gpio2,
  Gpio3,
  Gpio4,
  GpioCount,
};

enum class GpioMode {
  InputPullDown = 0,
  InputPullUp,
  InputNoPull,
  OutputPushPull,
  OutputOpenDrain,
};

class Gpio {
 public:
  Gpio(GPIO_TypeDef* port, uint16_t pin, GpioId id, GpioMode mode);
  virtual ~Gpio();

  void config(GpioMode mode);

  bool read_pin();
  void write_pin(bool state);

 private:
  void config_input_pulldown();
  void config_input_pullup();
  void config_input();
  void config_output_pushpull();
  void config_output_opendrain();

  GPIO_TypeDef* port_;
  uint16_t pin_;
  GpioId id_;
  GpioMode mode_;
};

}  // namespace hal::gpio

#endif /* HAL_GPIO_GPIO_HPP_ */
