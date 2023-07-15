/*
 * GpioIrq.cpp
 *
 *  Created on: Jul 9, 2023
 *      Author: Alexander L.
 */

#include "hal/gpio/GpioIrq.hpp"
#include "srv/debug.hpp"

#define DEBUG_ENABLE_GPIO_IRQ
#ifdef DEBUG_ENABLE_GPIO_IRQ
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][GpioIrq]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][GpioIrq]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][GpioIrq]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace hal::gpio {

GpioIrq::GpioIrq() {}

void GpioIrq::registerRequestSrvCallback(app::ctrl::RequestSrvCallback request_service_cb) {
  request_service_cb_ = request_service_cb;
}

Status_t GpioIrq::registerGpio(Gpio* gpio) {
  Status_t status;

  // Check if already registered
  for (size_t i = 0; i < registered_; i++) {
    if (gpio_[i] == gpio) {
      return Status_t::Ok;
    }
  }

  if ((gpio != nullptr) && (registered_ < sizeof(gpio_))) {
    DEBUG_INFO("Register gpio(%d) [ok]", gpio->id_)

    gpio_[registered_] = gpio;
    HAL_NVIC_EnableIRQ(gpio->irq_);

    registered_++;
    status = Status_t::Ok;

  } else {
    DEBUG_INFO("Register gpio(%d) [failed]", gpio->id_)
    status = Status_t::Error;
  }

  return status;
}

Status_t GpioIrq::deregisterGpio(Gpio* gpio) {
  Status_t status = Status_t::Ok;
  bool shift = false;

  // Find gpio in array
  for (size_t i = 0; i < registered_; i++) {
    if (gpio_[i] == gpio) {
      // Deregister gpio
      HAL_NVIC_DisableIRQ(gpio->irq_);
      shift = true;
    }

    if (shift == true) {
      gpio_[i] = gpio_[i + 1];
    }
  }

  if (shift == true) {
    registered_--;
    gpio_[registered_] = nullptr;

    DEBUG_INFO("Deregister gpio(%d) [ok]", gpio->id_)
  }

  return status;
}

void GpioIrq::extiCb(uint16_t pin) {
  for (size_t i = 0; i < registered_; i++) {
    if (gpio_[i]->pin_ == pin) {
      gpio_[i]->extiCb();
      break;
    }
  }

  // Request to be serviced by ctrlTask
  if (request_service_cb_ != nullptr) {
    request_service_cb_(1);
  }
}

extern "C" {
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  GpioIrq::getInstance().extiCb(GPIO_Pin);
}
}  // extern "C"

}  // namespace hal::gpio
