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
  NotInitialized,
};

class Gpio {
 public:
  Gpio(GPIO_TypeDef* port, uint16_t pin, IRQn_Type irq, GpioId id);
  virtual ~Gpio();

  Status_t config(GpioMode mode);
  bool readPin();
  void writePin(bool state);

 private:
  void configInputPullDown();
  void configInputPullUp();
  void configInput();
  void configOutputPushPull();
  void configOutputOpenDrain();
  void extiCb();

  GPIO_TypeDef* port_;
  uint16_t pin_;
  IRQn_Type irq_;
  GpioId id_;
  GpioMode mode_ = GpioMode::NotInitialized;

  bool in_interrupt_read_ = false;
  bool in_interrupt_state_ = false;

  friend class GpioIrq;
};

}  // namespace hal::gpio

#endif /* HAL_GPIO_GPIO_HPP_ */
