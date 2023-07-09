/*
 * Gpio.cpp
 *
 *  Created on: May 6, 2023
 *      Author: Alexander L.
 */

#include "hal/gpio/Gpio.hpp"
#include "srv/debug.hpp"

#define DEBUG_ENABLE_GPIO
#ifdef DEBUG_ENABLE_GPIO
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][Gpio]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][Gpio]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][Gpio]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace hal::gpio {

Gpio::Gpio(GPIO_TypeDef* port, uint16_t pin, GpioId id) : port_{ port }, pin_{ pin }, id_{ id } {}

Gpio::~Gpio() {}

void Gpio::config(GpioMode mode) {
  if (mode_ == mode) {
    return;
  }

  switch (mode) {
    case GpioMode::InputPullDown: {
      config_input_pulldown();
      break;
    }
    case GpioMode::InputPullUp: {
      config_input_pullup();
      break;
    }
    case GpioMode::InputNoPull: {
      config_input();
      break;
    }
    case GpioMode::OutputPushPull: {
      config_output_pushpull();
      break;
    }
    case GpioMode::OutputOpenDrain: {
      config_output_opendrain();
      break;
    }
    default: {
      break;
    }
  }

  DEBUG_INFO("Cfg pin: %d, mode: %d => %d", pin_, mode_, mode);
  mode_ = mode;
}

bool Gpio::read_pin() {
  auto state = HAL_GPIO_ReadPin(port_, pin_);
  DEBUG_INFO("Read pin: %d = %d", pin_, state);
  return static_cast<bool>(state);
}

void Gpio::write_pin(bool state) {
  if (mode_ >= GpioMode::OutputPushPull) {
    DEBUG_INFO("Write pin: %d = %d", pin_, state);
    HAL_GPIO_WritePin(port_, pin_, static_cast<GPIO_PinState>(state));
  }
}

void Gpio::config_input_pulldown() {
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  GPIO_InitStruct.Pin = pin_;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void Gpio::config_input_pullup() {
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  GPIO_InitStruct.Pin = pin_;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void Gpio::config_input() {
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  GPIO_InitStruct.Pin = pin_;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void Gpio::config_output_pushpull() {
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  GPIO_InitStruct.Pin = pin_;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void Gpio::config_output_opendrain() {
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  GPIO_InitStruct.Pin = pin_;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

}  // namespace hal::gpio
