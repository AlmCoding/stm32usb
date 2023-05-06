/*
 * Gpio.hpp
 *
 *  Created on: May 6, 2023
 *      Author: Alexander L.
 */

#ifndef HAL_GPIO_GPIO_HPP_
#define HAL_GPIO_GPIO_HPP_

namespace hal {
namespace gpio {

class Gpio {
 public:
  Gpio();
  virtual ~Gpio();
};

} /* namespace gpio */
} /* namespace hal */

#endif /* HAL_GPIO_GPIO_HPP_ */
