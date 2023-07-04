/*
 * GpioService.cpp
 *
 *  Created on: 1 Jul 2023
 *      Author: Alexander L.
 */

#include "app/gpio_srv/GpioService.hpp"
#include "os/msg/msg_broker.hpp"
#include "srv/debug.hpp"

#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "proto_c/gpio.pb.h"

#define DEBUG_ENABLE_GPIO_SERVICE
#ifdef DEBUG_ENABLE_GPIO_SERVICE
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][gpioSrv]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][gpioSrv]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][gpioSrv]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace app::gpio_srv {

GpioService::GpioService() {}

GpioService::~GpioService() {}

void GpioService::init() {
  gpio1_.config(hal::gpio::GpioMode::InputPullDown);
  gpio2_.config(hal::gpio::GpioMode::InputPullDown);
  gpio3_.config(hal::gpio::GpioMode::InputPullDown);
  gpio4_.config(hal::gpio::GpioMode::InputPullDown);
}

uint32_t GpioService::poll() {
  return 0;
}

int32_t GpioService::postRequest(const uint8_t* data, size_t len) {
  /* Allocate space for the decoded message. */
  gpio_proto_GpioMsg gpio_msg = gpio_proto_GpioMsg_init_zero;
  /* Create a stream that reads from the buffer. */
  pb_istream_t stream = pb_istream_from_buffer(data, len);

  /* Now we are ready to decode the message. */
  if (pb_decode(&stream, gpio_proto_GpioMsg_fields, &gpio_msg) == false) {
    DEBUG_ERROR("ProtoBuf decode [failed]");
    return -1;
  }

  if (gpio_msg.which_msg == gpio_proto_GpioMsg_data_msg_tag) {
    gpio1_.write_pin(gpio_msg.msg.data_msg.gpio1);
    gpio2_.write_pin(gpio_msg.msg.data_msg.gpio2);
    gpio3_.write_pin(gpio_msg.msg.data_msg.gpio3);
    gpio4_.write_pin(gpio_msg.msg.data_msg.gpio4);

  } else if (gpio_msg.which_msg == gpio_proto_GpioMsg_cfg_msg_tag) {
    gpio1_.config(static_cast<hal::gpio::GpioMode>(gpio_msg.msg.cfg_msg.gpio1));
    gpio2_.config(static_cast<hal::gpio::GpioMode>(gpio_msg.msg.cfg_msg.gpio2));
    gpio3_.config(static_cast<hal::gpio::GpioMode>(gpio_msg.msg.cfg_msg.gpio3));
    gpio4_.config(static_cast<hal::gpio::GpioMode>(gpio_msg.msg.cfg_msg.gpio4));
  }

  seqence_number_ = gpio_msg.sequence_number;
  return 0;
}

int32_t GpioService::serviceRequest(uint8_t* data, size_t max_len) {
  /* Allocate space for the decoded message. */
  gpio_proto_GpioMsg gpio_msg = gpio_proto_GpioMsg_init_zero;
  /* Create a stream that will write to our buffer. */
  pb_ostream_t stream = pb_ostream_from_buffer(data, max_len);

  gpio_msg.sequence_number = seqence_number_;
  gpio_msg.which_msg = gpio_proto_GpioMsg_data_msg_tag;

  gpio_msg.msg.data_msg.gpio1 = gpio1_.read_pin();
  gpio_msg.msg.data_msg.gpio2 = gpio2_.read_pin();
  gpio_msg.msg.data_msg.gpio3 = gpio3_.read_pin();
  gpio_msg.msg.data_msg.gpio4 = gpio4_.read_pin();

  DEBUG_INFO("Srv data (seq: %d)", gpio_msg.sequence_number);
  return stream.bytes_written;
}

}  // namespace app::gpio_srv
